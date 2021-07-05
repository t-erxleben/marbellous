#include <emscripten.h>
#include <cstdio>
#include <cmath>

#include <stdlib.h> 

#include "WGLContext.hpp"
#include "WGLSceneRenderer.hpp"
#include "Scene.hpp"
#include "Options.hpp"
#include "WGLRakeRenderer.hpp"

#define checkSetup(RET) if (!setupDone) \
        { \
            fprintf(stderr, "%s: %s", __func__, ": Backend is not initialized yet!\n"); \
            return RET; \
        } \

bool setupDone = false;
WGLSceneRenderer *sceneRenderer;
Scene *scene;

void _initWGLContext(char *canvasID, size_t x, size_t y)
{
    WGLContext::instance = new WGLContext(canvasID, x, y);
}

/*------------------------------------------
Interface to front end:
--------------------------------------------*/

extern "C"
{
    void initWGLContext(char *canvasID, size_t x, size_t y)
    {
        _initWGLContext(canvasID, x, y);
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
        Options::getInstance()->setBGColor(Color{color});
		if(WGLContext::instance) {
			WGLContext::instance->updateBGColor();
		}
    }

    int EMSCRIPTEN_KEEPALIVE addDrop(float const x, float const y, float r, unsigned int const color) ///< draw a circle at point (x,y) (should be normed to [-1,1]^2) with radius r in the given color
    {
        checkSetup(-1);

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
		sceneRenderer->drawScene(*scene);
	}

    int EMSCRIPTEN_KEEPALIVE resizeDrop(int const dropID, float const newRadius) ///< resize drop of given ID (return val of addDrop(...))
    {
        checkSetup(-1);
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
		scene->applyDisplacement();
		sceneRenderer->drawScene(*scene);
		return 0;
	}

    char *EMSCRIPTEN_KEEPALIVE getImage()
    {
		constexpr int X = 720;
		constexpr int Y = 720;
        checkSetup(NULL);
        static std::vector<char> data(X * Y * 4 + sizeof(int)*2);
		reinterpret_cast<int*>(data.data())[0] = X;
		reinterpret_cast<int*>(data.data())[1] = Y;

        sceneRenderer->drawToBuffer(*scene, data.data() + sizeof(int)*2, static_cast<int>(data.size() - sizeof(int)*2));
        char *ptr = data.data() + sizeof(int)*2;
        for (int i = 0; i < X * Y; ++i)
        {
            ptr[3 * i] = ptr[4 * i];
            ptr[3 * i + 1] = ptr[4 * i + 1];
            ptr[3 * i + 2] = ptr[4 * i + 2];
        }
		for(int y = 0; y < Y/2; ++y) {
			for(int x = 0; x < X; ++x) {
				int i = y * X + x;
				int j = (Y - y - 1) * X + x;
				std::swap(ptr[3* i], ptr[3*j]);
				std::swap(ptr[3* i + 1], ptr[3*j + 1]);
				std::swap(ptr[3* i + 2], ptr[3*j + 2]);
			}
		}
        return data.data();
    }
  
    void EMSCRIPTEN_KEEPALIVE dropColor(float const x, float const y, float const r, unsigned int const color) ///< deprecated and only draws in color 0
    {
        checkSetup();
        if (r > Polygon::MIN_R)
        {
            scene->addPolygon(Polygon{Point{x, y}, r, 0});
            sceneRenderer->drawScene(*scene);
        }
    }

	void EMSCRIPTEN_KEEPALIVE clearCanvas() { ///< clear the canvas (delete all polygones and redraw scene)
		checkSetup();
		scene->clear();
		sceneRenderer->drawScene(*scene);
	}

	/** execute a linear rake in direction <x,y> with speed = ||<x,y>||. 
    * <nails> is an array of bool with are the nails from begin to end of the rake. a 1 means there is a nail, 0 means thar is not.
    */
	void EMSCRIPTEN_KEEPALIVE rakeLinear(float x, float y, bool nails[1000]) {

		// TODO: implement 
		float len = sqrt(x*x+y*y);
		std::cerr << "Rake: dir(" << x/len << ", " << y/len << ") with " << len << "\n";
		GLuint nail_uint[1000];
		for(int i = 0; i < 1000; ++i) { nail_uint[i] = nails[i] ? 1 : 0; }
		int count = 0;
        for(int i = 0; i <1000; ++i) {
			if(nail_uint[i] == 0) { ++count; }
			else {
				std::cout << "<"<<count<<">"<< 1 << ", ";
				count = 0;
			}
		}
        std::cout << std::endl;
	}


	void EMSCRIPTEN_KEEPALIVE startRaking() {
		// TODO: implement
		std::cout << "start Raking\n";
	}
/*------------------------------------------
Init stuff:
--------------------------------------------*/

    int EMSCRIPTEN_KEEPALIVE main()
    {
        // setup

        ///This needs to be done in the front end (init call should be after bg):
        char id[] = "#image";
        initWGLContext(id, 720, 720);
		WGLContext::instance->updateBGColor();

        sceneRenderer = new WGLSceneRenderer{};
        scene = new Scene{};

        setupDone = true;

        // keep WASM module alive
        EM_ASM(Module['noExitRuntime'] = true);
        return 0;
    }
}
