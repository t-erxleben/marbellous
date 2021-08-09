#include <emscripten.h>
#include <cstdlib> 
#include <cstdio>
#include <cmath>
#include <random>

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

	static std::mt19937 rng(std::random_device{}());

	auto dropRes = WGLContext::getContext()->getDropRes();

	sceneRenderer->drawScene(*scene);

	for(int i = 0; i < amt; ++i) {
		Point p = coord(rng);
		float r = radius(rng);
		GLuint col = static_cast<GLuint>(Options::getInstance()->getActivePalette()->getRandomColorId());

		scene->applyDisplacement();
        r = r>=Polygon::MIN_R ? r : Polygon::MIN_R;
		scene->addDisplacement({p.x,p.y}, r);

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

struct DropData {
	float x;
	float y;
	float r;
	float colorId;
};

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

		p->setColorAt(colorNumber, Color(color));
        return 0;        
    }

	int EMSCRIPTEN_KEEPALIVE setColorRatioAt(size_t const colorNumber, unsigned const ratio)
	{
		Palette& p = *Options::getInstance()->getActivePalette();
		if(p.getSize() <= colorNumber) { return -1; }
		p.setRatioAt(colorNumber, ratio);
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
		scene->addDisplacement({x,y}, r);

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

	void EMSCRIPTEN_KEEPALIVE addDrops(int count, DropData drops[])
	{
		checkSetup();
		checkState(true, );

		auto dropRes = WGLContext::getContext()->getDropRes();

		scene->applyDisplacement();
		sceneRenderer->drawScene(*scene);

		uint8_t c[4];
		for(int i = 0; i < count; ++i) {
			DropData& drop = drops[i];
			scene->addDisplacement({drop.x,drop.y}, std::max(Polygon::MIN_R, drop.r));
			GLuint color = drop.colorId >= 0
				? static_cast<GLuint>(drop.colorId)
				: Options::getInstance()->getActivePalette()->getRandomColorId();
			glReadPixels((drop.x+1.f)*dropRes/2, (drop.y+1.f)*dropRes/2, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, c);
			const auto& [cr, cg, cb] = (*Options::getInstance()->getActivePalette())[color].getRGB();
			if(cr == c[0] && cg == c[1] && cb == c[2]) { continue; }
			scene->addPolygon({{drop.x,drop.y}, Polygon::MIN_R, color});
		}
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

    void EMSCRIPTEN_KEEPALIVE resize(size_t dropSize, size_t rakeSize)
    {
        /// \todo for now both need to be the same
        assert(dropSize == rakeSize);
        auto context = WGLContext::getContext();
        context->setDropRes(dropSize);
        context->setRakeRes(rakeSize);
        sceneRenderer->resize();
        rakeRenderer->resize();
        Options::getInstance()->setState(true);
        redraw();
    }

    int EMSCRIPTEN_KEEPALIVE resizeDrops(float const newRadius) ///< resize all drops
    {
        checkSetup(-1);
        checkState(true, -1);

		scene->setDisplacementRadius(newRadius);
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

    /**
     * Rake paint on the canvas. Wraps around all edges.
     * 
     * \param x Stroke strength in x direction.
     * \param y Stroke strength in y direction.
     * \param period Period length of waves given as factor for canvas size.
     * \param amplitude Amplitude of waves given as factor for canvas size.
     * \param phase Phase shift of waves given as factor for canvas size.
     * \param nails Boolean array which describes the rake.
     */
    void EMSCRIPTEN_KEEPALIVE rake(float x, float y, float period, float amplitude, float phase, bool nails[1000]) {
        checkState(false,);

		GLuint nail_uint[1000];

        // account for different defintions of origin
        if(abs(y) < 1e-9)
        {
		    for(int i = 0; i < 1000; ++i) { nail_uint[i] = nails[999 - i] ? 1 : 0; }
        }
        else
        {
		    for(int i = 0; i < 1000; ++i) { nail_uint[i] = nails[i] ? 1 : 0; }
        }


        float xs =  0.5f * x;
        float ys = -0.5f * y;

        rakeRenderer->rake(x, -1.*y, period, amplitude, phase, nail_uint);
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
