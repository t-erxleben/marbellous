#include <emscripten.h>
#include <cstdio>
#include <cmath>
#include <random>

#include <stdlib.h> 

#include "WGLContext.hpp"
#include "WGLSceneRenderer.hpp"
#include "WGLRakeRenderer.hpp"
#include "Scene.hpp"
#include "Options.hpp"
#include "WGLRakeRenderer.hpp"

#define checkSetup(RET) if (!setupDone) \
        { \
            fprintf(stderr, "%s: %s", __func__, ": Backend is not initialized yet!\n"); \
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


void _initWGLContext(const char *canvasID, size_t x)
{
    WGLContext::instance = new WGLContext(canvasID, x);
}

extern "C" {
	int EMSCRIPTEN_KEEPALIVE addDrop(float,float,float,unsigned);
	int EMSCRIPTEN_KEEPALIVE finishDrop(int); }

template<typename C, typename R>
void sprinkle(int amt, C& coord, R& radius)
{
	checkSetup();
	checkState(true,);

	static constexpr unsigned int colorRoom = 10 * 9 * 8 * 7 * 6 * 5;
	static std::mt19937 rng(std::random_device{}());
	static std::uniform_int_distribution<int> color(0, colorRoom - 1);

	auto dropRes = WGLContext::getContext()->getDropRes();

	sceneRenderer->drawScene(*scene);

	for(int i = 0; i < amt; ++i) {
		Point p = coord(rng);
		float r = radius(rng);
		GLuint col = static_cast<GLuint>(color(rng)
				% Options::getInstance()->getActivePalette()->getSize());

		scene->applyDisplacement();
        r = r>=Polygon::MIN_R ? r : Polygon::MIN_R;
		scene->setDisplacement({p.x,p.y}, r);

		uint8_t c[4];
		glReadPixels((p.x+1.f)*dropRes/2, (p.y+1.f)*dropRes/2, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, c);
		const auto& [cr,cg,cb] = (*Options::getInstance()->getActivePalette())[col].getRGB();
		if(cr == c[0] && cg == c[1] && cb == c[2]) {
			continue;
		}

        int handle = scene->addPolygon(Polygon{p, Polygon::MIN_R,
				col});
	}
	scene->applyDisplacement();
	sceneRenderer->drawScene(*scene);
}

/*------------------------------------------
Interface to front end:
--------------------------------------------*/

extern "C"
{
    void EMSCRIPTEN_KEEPALIVE initBackend(const char canvasID[], size_t dropRes, size_t rakeRes)
    {
        _initWGLContext(canvasID, dropRes);

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

    int EMSCRIPTEN_KEEPALIVE addDrop(float const x, float const y, float r, unsigned int const color) ///< draw a circle at point (x,y) (should be normed to [-1,1]^2) with radius r in the given color
    {
        checkSetup(-1);
        checkState(true, -1);

        auto dropRes = WGLContext::getContext()->getDropRes();

		scene->applyDisplacement();
        r = r>=Polygon::MIN_R ? r : Polygon::MIN_R;
		scene->setDisplacement({x,y}, r);

        sceneRenderer->drawScene(*scene);

		uint8_t c[4];
		glReadPixels((x+1.f)*dropRes/2, (y+1.f)*dropRes/2, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, c);
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

    int EMSCRIPTEN_KEEPALIVE resizeDrops(float const newRadius) ///< resize all drops
    {
        checkSetup(-1);
        checkState(true, -1);

		scene->setDisplacement(scene->getDisplacement().p, newRadius);
        sceneRenderer->drawScene(*scene);
    
        return 0;
    }

	/// finalize all current growing drops
	int EMSCRIPTEN_KEEPALIVE finishDrops()
	{
		checkSetup(-1);
        checkState(true, -1);

		scene->applyDisplacement();
		sceneRenderer->drawScene(*scene);
		return 0;
	}

	/// create drops in grid pattern
	/**
	 * \param x,y handle position on canvas
	 * \param colorId of color of drops
	 * \param w,h width and height of one pattern cell
	 * \param of offset betwen two columns of the pattern
	 */
	void EMSCRIPTEN_KEEPALIVE addGridDrops(float x, float y,
			float w, float h, float of, unsigned colorId)
	{}

    char *EMSCRIPTEN_KEEPALIVE getImage()
    {
        checkSetup(NULL);

        static std::vector<char> data;
        size_t canvasRes;

        if(Options::getInstance()->getState())
        {
            // drop
            canvasRes = WGLContext::getContext()->getDropRes();
            data.resize(canvasRes * canvasRes * 4 + 2*sizeof(int));
            sceneRenderer->drawToBuffer(*scene, data.data() + sizeof(int)*2, static_cast<int>(data.size() - sizeof(int)*2));
        }
        else
        {
            // rake
            canvasRes = WGLContext::getContext()->getRakeRes();
            data.resize(canvasRes * canvasRes * 4 + 2*sizeof(int));
            rakeRenderer->drawToBuffer(data.data() + sizeof(int)*2, static_cast<int>(data.size() - sizeof(int)*2));
        }

		reinterpret_cast<int*>(data.data())[0] = canvasRes;
		reinterpret_cast<int*>(data.data())[1] = canvasRes;

        char *ptr = data.data() + sizeof(int)*2;
        for (int i = 0; i < canvasRes * canvasRes; ++i)
        {
            ptr[3 * i] = ptr[4 * i];
            ptr[3 * i + 1] = ptr[4 * i + 1];
            ptr[3 * i + 2] = ptr[4 * i + 2];
        }
		for(int y = 0; y < canvasRes/2; ++y) {
			for(int x = 0; x < canvasRes; ++x) {
				int i = y * canvasRes + x;
				int j = (canvasRes - y - 1) * canvasRes + x;
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
        checkState(true,);
        if (r > Polygon::MIN_R)
        {
            scene->addPolygon(Polygon{Point{x, y}, r, 0});
            sceneRenderer->drawScene(*scene);
        }
    }

	/// creates sprinkle in around cursor
	/**
	 * \param amt number of drops to create
	 * \param r_min,r_max drop size range
	 * \param x,y cursor position
	 * \param sig sigma for normal distribution for drop position
	 */
	void EMSCRIPTEN_KEEPALIVE sprinkleLocal(int amt, float r_min, float r_max,
			float x, float y, float sig)
	{
		checkSetup();
		checkState(true,);

		auto coord = [&](auto& prng) {
			std::normal_distribution<float> dis(0, sig);
			return Point{ dis(prng) + x, dis(prng) + y };
		};
		std::uniform_real_distribution<float> radius(r_min, r_max);
		sprinkle(amt, coord, radius);
	}
	void EMSCRIPTEN_KEEPALIVE sprinkleGlobal(int amt, float r_min, float r_max) {
		checkSetup();
		checkState(true,);

		static std::mt19937 rng(std::random_device{}());
		static auto coord = [](auto& prng) {
	 		std::uniform_real_distribution<float> dis(-1.f, 1.f);
			return Point {dis(prng), dis(prng)};
		};

		std::uniform_real_distribution<float> radius(r_min, r_max);
		sprinkle(amt, coord, radius);
	}

	void EMSCRIPTEN_KEEPALIVE clearCanvas() { ///< clear the canvas (delete all polygones and redraw scene)
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

	/** execute a linear rake in direction <x,y> with speed = ||<x,y>||. 
    * <nails> is an array of bool with are the nails from begin to end of the rake. a 1 means there is a nail, 0 means thar is not.
    */
	void EMSCRIPTEN_KEEPALIVE rakeLinear(float x, float y, bool nails[1000]) {
        checkState(false,);

		float len = sqrt(x*x+y*y);
		std::cerr << "Rake: dir(" << x/len << ", " << y/len << ") with " << len << "\n";

		GLuint nail_uint[1000];
		for(int i = 0; i < 1000; ++i) { nail_uint[i] = nails[i] ? 1 : 0; }

        rakeRenderer->rake(x,-1.*y,nail_uint);
        rakeRenderer->draw();
	}

	void EMSCRIPTEN_KEEPALIVE startRaking() {
        checkState(true,);
		std::cout << "start Raking\n";
        Options::getInstance()->setState(false);
        rakeRenderer->reset(*sceneRenderer, *scene);
        rakeRenderer->draw();
	}

    void EMSCRIPTEN_KEEPALIVE startDropping() {
        checkState(false,);
		std::cout << "start Dropping\n";
        Options::getInstance()->setState(true);
        sceneRenderer->drawScene(*scene);
	}

/*------------------------------------------
Init stuff:
--------------------------------------------*/

    int EMSCRIPTEN_KEEPALIVE main()
    {
        // keep WASM module alive
        EM_ASM(Module['noExitRuntime'] = true);
        return 0;
    }
}
