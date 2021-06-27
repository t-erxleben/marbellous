#include <emscripten.h>
#include <cstdio>
#include <cmath>

#include <stdlib.h> 

#include "WGLContext.hpp"
#include "WGLSceneRenderer.hpp"
#include "WGLRakeRenderer.hpp"
#include "Scene.hpp"
#include "Options.hpp"
#include "WGLRakeRenderer.hpp"

#define checkSetup(RET) if (!setupDone) \
        { \
            fprintf(stderr, "Backend is not initialized yet!\n"); \
            return RET; \
        } \

#define checkState(STATE, RET) if(Options::getInstance()->getState() != STATE) \
        { \
            fprintf(stderr, "Drop function called in rake state or rake function called in drop state!\n"); \
            return RET; \
        } \

bool setupDone = false;
WGLSceneRenderer *sceneRenderer;
WGLRakeRenderer *rakeRenderer;
Scene *scene;

void _initWGLContext(char *canvasID, size_t x)
{
    WGLContext::instance = new WGLContext(canvasID, x);
}

/*------------------------------------------
Interface to front end:
--------------------------------------------*/

extern "C"
{
    void initBackend(char *canvasID, size_t x)
    {
        _initWGLContext(canvasID, x);

		WGLContext::getContext()->updateBGColor();

        sceneRenderer = new WGLSceneRenderer{};
        scene = new Scene{};
        rakeRenderer = new WGLRakeRenderer(*sceneRenderer, *scene);

        setupDone = true;
    }

    int EMSCRIPTEN_KEEPALIVE addPalette(size_t num_colors)
    {
        return Options::getInstance()->addPalette(Palette{num_colors});
    }

    int EMSCRIPTEN_KEEPALIVE setActivePalette(int const id)
    {
        return Options::getInstance()->setActivePalette(id);
    }

    int EMSCRIPTEN_KEEPALIVE setColorAt(size_t const colorNumber, unsigned int const color)
    {
        Palette* p = Options::getInstance()->getActivePalette();
        if(p->getSize() <= colorNumber) return -1;
        (*p)[colorNumber] = Color{color};
        return 0;        
    }

    int EMSCRIPTEN_KEEPALIVE setPaletteColors(unsigned int const c0, unsigned int const c1, unsigned int const c2, unsigned int const c3)
    {
        Palette* p = Options::getInstance()->getActivePalette();
        
        // add missing colors
        int num_colors_missing = 4 - p->getSize();
        for(int i = 0; i < num_colors_missing; ++i) p->add(Color{});

        setColorAt(0, c0);
        setColorAt(1, c1);
        setColorAt(2, c2);
        setColorAt(3, c3);
        return 0;
    }

    void EMSCRIPTEN_KEEPALIVE setBGColor(unsigned int const color)
    {
        checkSetup();
        Options::getInstance()->setBGColor(Color{color});
        auto context = WGLContext::getContext();
		context->updateBGColor();
    }

    // draw a circle at point (x,y) (should be normed to [-1,1]^2) with radius r in the given color
    int EMSCRIPTEN_KEEPALIVE addDrop(float const x, float const y, float r, unsigned int const color)
    {
        checkSetup(-1);
        checkState(true, -1);

		scene->applyDisplacement();
        r = r>=Polygon::MIN_R ? r : Polygon::MIN_R;
		scene->setDisplacement({x,y}, r);

        sceneRenderer->drawScene(*scene);

		uint8_t c[4];
		glReadPixels((x+1.f)*720/2, (y+1.f)*720/2, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, c);
		const auto& [cr,cg,cb] = (*Options::getInstance()->getActivePalette())[color].getRGB();
		if(cr == c[0] && cg == c[1] && cb == c[2]) {
			return -1;
		}
        int handle = scene->addPolygon(Polygon{Point{x,y}, Polygon::MIN_R, color});
        return handle;
    }

	void EMSCRIPTEN_KEEPALIVE redraw()
	{
		checkSetup();
        auto drop = Options::getInstance()->getState();

        if(drop)
        {
		    sceneRenderer->drawScene(*scene);
        }
        else
        {
            rakeRenderer->draw();
        }
	}

    // resize drop of given ID (return val of addDrop(...))
    // influence of resizeVal is not defined yet
    int EMSCRIPTEN_KEEPALIVE resizeDrop(int const dropID, float const newRadius)
    {
        checkSetup(-1);
        checkState(true, -1);

		if(dropID >= 0 && dropID != scene->getPolygonCount() - 1) {
			fprintf(stderr, "Can only resize the last added drop!\n\tgot: %i, expected: %lu",
					dropID,
					scene->getPolygonCount() - 1);
			return -1;
		}
		scene->setDisplacement(scene->getDisplacement().p, newRadius);
        sceneRenderer->drawScene(*scene);
    
        return 0;
    }

	int EMSCRIPTEN_KEEPALIVE finishDrop(int dropID)
	{
		checkSetup(-1);
        checkState(true, -1);

		scene->applyDisplacement();
		sceneRenderer->drawScene(*scene);
		return 0;
	}

    char *EMSCRIPTEN_KEEPALIVE getImage()
    {
		constexpr int X = 720;
		constexpr int Y = 720;
        checkSetup(NULL);
        constexpr char prefix[] = "P6\n720\n720\n255\n";
        constexpr int prefix_len = sizeof(prefix) - 1;
        static std::vector<char> data(X * Y * 4 + prefix_len + 1);
        data[0] = prefix_len;
        memcpy(data.data() + 1, prefix, prefix_len);

        auto drop = Options::getInstance()->getState();
        if(drop)
        {
            sceneRenderer->drawToBuffer(*scene, data.data() + prefix_len + 1, static_cast<int>(data.size() - prefix_len - 1));
        }
        else
        {
            rakeRenderer->drawToBuffer(data.data() + prefix_len + 1, static_cast<size_t>(data.size() - prefix_len - 1));
        }

        char *ptr = data.data() + prefix_len + 1;
        for (int i = 0; i < X * Y; ++i)
        {
            ptr[3 * i] = ptr[4 * i];
            ptr[3 * i + 1] = ptr[4 * i + 1];
            ptr[3 * i + 2] = ptr[4 * i + 2];
        }
		for(int y = 0; y < Y/2; ++y) {
			for(int x = 0; x < X; ++x) {
				int i = y * X + x;
				int j = (Y-y) * X + x;
				std::swap(ptr[3* i], ptr[3*j]);
				std::swap(ptr[3* i + 1], ptr[3*j + 1]);
				std::swap(ptr[3* i + 2], ptr[3*j + 2]);
			}
		}
        return data.data();
    }
  
    // deprecated and only draws in color 0
    void EMSCRIPTEN_KEEPALIVE dropColor(float const x, float const y, float const r, unsigned int const color)
    {
        checkSetup();
        checkState(true,);
        if (r > Polygon::MIN_R)
        {
            scene->addPolygon(Polygon{Point{x, y}, r, 0});
            sceneRenderer->drawScene(*scene);
        }
    }

	// clear the canvas (delete all polygones and redraw scene)
	void EMSCRIPTEN_KEEPALIVE clearCanvas() {
		checkSetup();
        
		scene->clear();
		sceneRenderer->drawScene(*scene);

        bool drop = Options::getInstance()->getState();
        if(!drop)
        {
            rakeRenderer->reset(*sceneRenderer, *scene);
            rakeRenderer->draw();
        } 
    }

	// execute a linear rake in direction <x,y> with speed <speed>. <nails> is an array of bool with
	// are the nails from begin to end of the rake. a 1 means there is a nail, 0 means thar is not.
	void EMSCRIPTEN_KEEPALIVE rakeLinear(float x, float y, float speed, GLuint nails[1000]) {
        checkState(false,)

		std::cerr << "Rake: dir(" << x << ", " << y << ") with " << speed << "\n";
        constexpr float eps = 1e-9;

        // FIXME normalize direction and rescale speed in front end
        if(abs(x) > eps) x = (x < 0) ? -1. : 1.;
        if(abs(y) > eps) y = (y < 0) ? 1. : -1.;
        speed /= 5000.;

        // FIXME repair broken nails from front end
        static GLuint n[1000];
        for(int i = 0; i <1000; ++i) n[i] = i%150==0;

        printf("x: %f, y %f, s: %f\n", x,y, speed);

        rakeRenderer->rake(x,y,speed, n);
        rakeRenderer->draw();
	}

	void EMSCRIPTEN_KEEPALIVE startRaking() {
        checkState(true,)
		std::cout << "start Raking\n";
        Options::getInstance()->setState(false);
        rakeRenderer->reset(*sceneRenderer, *scene);
        rakeRenderer->draw();
	}

    void EMSCRIPTEN_KEEPALIVE startDropping() {
        checkState(false,)
		std::cout << "start Dropping\n";
        Options::getInstance()->setState(true);
        sceneRenderer->drawScene(*scene);
	}
/*------------------------------------------
Init stuff:
--------------------------------------------*/

    int EMSCRIPTEN_KEEPALIVE main()
    {
        // setup
        // This needs to be done in the front end (init call should be after bg):
        char id[] = "#image";
        initBackend(id, 720);

        // keep WASM module alive
        EM_ASM(Module['noExitRuntime'] = true);
        return 0;
    }
}
