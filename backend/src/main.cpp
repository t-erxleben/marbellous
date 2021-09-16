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

/**
 * \brief Check wether the backend is initialized or not.
 * 
 * \param RET return value of the function in case that the backend is not initialized.
 **/
#define checkSetup(RET) if (!setupDone) \
        { \
            fprintf(stderr, "%s: %s", __func__, ": Backend is not initialized yet!\n"); \
            return RET; \
        } \

/**
 * \brief Check wether the backend is currently in the state given by \p STATE
 * 
 * \param STATE State to compare with. True for drop state, false for rake state
 * \param RET Return value of the function in case of unmatched state.
 * 
 **/
#define checkState(STATE, RET) if(Options::getInstance()->getState() != STATE) \
        { \
            fprintf(stderr, "Drop function called in rake state or rake function called in drop state!\n"); \
            return RET; \
        } \

///< Stores wether backend is initialized or not
bool setupDone = false;

///< global WGLSceneRenderer
WGLSceneRenderer *sceneRenderer;

///< global scene
Scene *scene;

///< global WGLRakeRenderer
WGLRakeRenderer *rakeRenderer;

/**
 * \brief Used to initialize the WGL context.
 * 
 * Friend function of WGLContext
 * 
 * \param canvasID CSS identifier of the HTML5 canvas to use.
 * \param x Resolution of the canvas will be set to x times x.
 **/
void _initWGLContext(const char *canvasID, size_t x)
{
    WGLContext::instance = new WGLContext(canvasID, x);
}

extern "C" 
{
    // used in sprinkle
	int EMSCRIPTEN_KEEPALIVE addDrop(float,float,float,unsigned);
	int EMSCRIPTEN_KEEPALIVE finishDrop(int); 
}

/**
 * \brief Generate drops of random size and position.
 * 
 * \tparam C A function type for transforming random numbers to drop coordinates.
 * \tparam R A function type for transforming random numbers to drop sizes.
 * \param amt Number of drops to generate
 * \param coord A function of type C.
 * \param radius A function of type R.
 **/
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

		scene->applyDisplacement(dropRes);
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
    scene->applyDisplacement(dropRes);
    sceneRenderer->drawScene(*scene);
}

/*------------------------------------------
Interface to front end:
--------------------------------------------*/

/// All relevant data for a single drop grouped as a struct.
struct DropData {
	float x;
	float y;
	float r;
	float colorId;
};

extern "C"
{
    /**
     * \brief Init function. Should be called first.
     * 
     * \param canvasID CSS identifier of HTML5 canvas in the frontend.
     * \param dropRes Resolution of the canvas for drop mode.
     * \param rakeRes Resolution of the canvas for rake mode.
     * 
     * \attention Currently \p dropRes and \p rakeRes need to be the same.
     **/
    void EMSCRIPTEN_KEEPALIVE initBackend(const char canvasID[], size_t dropRes, size_t rakeRes)
    {
		assert(dropRes == rakeRes);

        _initWGLContext(canvasID, dropRes);

		WGLContext::getContext()->updateBGColor();

        sceneRenderer = new WGLSceneRenderer{};
        scene = new Scene{};
        rakeRenderer = new WGLRakeRenderer(*sceneRenderer, *scene);

        setupDone = true;
    }

    /**
     * \brief Add a new palette.
     * 
     * \param num_colors Number of colors to pre allocate.
     * \return int ID of this palette. May be used to set it active later.
     **/
    int EMSCRIPTEN_KEEPALIVE addPalette(size_t num_colors)
    {
        return Options::getInstance()->addPalette(Palette{num_colors});
    }

    /**
     * \brief Set an active palette.
     * 
     * \param id ID of the palette to set active. Return value of addPalette()
     * \return int Negative in case of failure.
     **/
    int EMSCRIPTEN_KEEPALIVE setActivePalette(int const id)
    {
        return Options::getInstance()->setActivePalette(id);
    }

    /**
     * \brief Set the number at position \p colorNumber of the active palette.
     * 
     * \param colorNumber Number of color to set.
     * \param color Color als HEX code.
     * \return int Negative in case of failure.
     **/
    int EMSCRIPTEN_KEEPALIVE setColorAt(size_t const colorNumber, unsigned int const color)
    {
        Palette* p = Options::getInstance()->getActivePalette();
        if(p->getSize() <= colorNumber) return -1;

		p->setColorAt(colorNumber, Color(color));
        return 0;        
    }

    /**
     * \brief Set the the ratio of a color in the active palette.
     * 
     * \param colorNumber Number of the color to modify.
     * \param ratio New ratio for sprinkler drops.
     * \return int Negative in case of failure.
     **/
	int EMSCRIPTEN_KEEPALIVE setColorRatioAt(size_t const colorNumber, unsigned const ratio)
	{
		Palette& p = *Options::getInstance()->getActivePalette();
		if(p.getSize() <= colorNumber) { return -1; }
		p.setRatioAt(colorNumber, ratio);
		return 0;
	}

    /**
     * \brief Set the background color.
     * 
     * \param color Color in HEX Code 
     **/
    void EMSCRIPTEN_KEEPALIVE setBGColor(unsigned int const color)
    {
        checkSetup();
        Options::getInstance()->setBGColor(Color{color});
        auto context = WGLContext::getContext();
		context->updateBGColor();
    }

    /**
     * \brief Set the filter option.
     * 
     * \param filter True for post processing filter, false otherwise.
     **/
    void EMSCRIPTEN_KEEPALIVE setFilter(bool filter)
    {
        Options::getInstance()->setFilter(filter);
    }

    /**
     * \brief Add a new color drop.
     * 
     * Intended workflow for animation is to add a drop, keep its ID and resize it step wise.
     * After the animation is done and the drop will not further grow call finishDrops().
     * 
     * \param x x coordinate for the drop.
     * \param y y coordinate for the drop.
     * \param r Radius of the drop.
     * \param color Color of th drop. This is a position inside a palette. Changing the active palette will result in a different color later on.
     * \return int Negative value in case of failure.
     **/
    int EMSCRIPTEN_KEEPALIVE addDrop(float const x, float const y, float r, unsigned int const color) ///< draw a circle at point (x,y) (should be normed to [-1,1]^2) with radius r in the given color
    {
        checkSetup(-1);
        checkState(true, -1);

        auto dropRes = WGLContext::getContext()->getDropRes();

		scene->applyDisplacement(dropRes);
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

    /**
     * \brief Add multiple drops at once.
     * 
     * \param count Number of drops.
     * \param drops Drop data.
     * 
     * \attention This is an unstable and experimental feature as it's hard to do with our model.
     **/
	void EMSCRIPTEN_KEEPALIVE addDrops(int count, DropData drops[])
	{
		checkSetup();
		checkState(true, );

		auto dropRes = WGLContext::getContext()->getDropRes();
		scene->applyDisplacement(dropRes);
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

    /**
     * \brief Redraw the current data model based on the backend state.
     * 
     * Useful for color updates for example.
     * 
     **/
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

    /**
     * \brief Resize the canvas.
     * 
     * \param dropSize New resolution for drop state.
     * \param rakeSize New resolution for rake state.
     * 
     * \attention Currently both need to be the same.
     **/
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

	/**
	 * \brief Finish all drops applying their displacement on all old drops.
	 * 
	 * \return int Negative value in case of failure.
	 **/
	int EMSCRIPTEN_KEEPALIVE finishDrops()
	{
		checkSetup(-1);
        checkState(true, -1);

		scene->applyDisplacement(WGLContext::getContext()->getDropRes());
		sceneRenderer->drawScene(*scene);
		return 0;
	}

    /**
     * \brief Return the current canvas content as image.
     * 
     * \return char* Image as raw pixel buffer.
     **/
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

	/**
     * \brief Create drops around the cursor postion.
     * 
	 * \param amt Number of drops to create.
	 * \param r_min,r_max drop size range
	 * \param x,y cursor position
	 * \param sig Sigma of normal distribution for drop position
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

    /**
     * \brief Create drops at random positions on the canvas
     * 
     * \param amt Number of drops to create.
     * \param r_min Minimum size.
     * \param r_max Maximum size.
     **/
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

    /**
     * \brief Clear canvas content and data model.
     * 
     * This can not be undone!
     **/
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
        checkSetup();
        checkState(false,);

		GLint nail_int[1000];

        // account for different defintions of origin
        if(abs(y) < 1e-9)
        {
		    for(int i = 0; i < 1000; ++i) { nail_int[i] = nails[999 - i] ? 1 : 0; }
        }
        else
        {
		    for(int i = 0; i < 1000; ++i) { nail_int[i] = nails[i] ? 1 : 0; }
        }


        float xs =  0.5f * x;
        float ys = -0.5f * y;

        rakeRenderer->rake(x, -1.*y, period, amplitude, phase, nail_int);
        rakeRenderer->draw();
	}

    /**
     * \brief Set rake state.
     * 
     * This results in the scene being rendered to a hidden buffer which is given to the WGLRakeRenderer
     * 
     **/
	void EMSCRIPTEN_KEEPALIVE startRaking() {
        checkState(true,);
        Options::getInstance()->setState(false);
        rakeRenderer->reset(*sceneRenderer, *scene);
        rakeRenderer->draw();
	}

    /**
     * \brief Set drop state.
     * 
     * All rakes will be lost.
     * 
     **/
    void EMSCRIPTEN_KEEPALIVE startDropping() {
        checkState(false,);
        Options::getInstance()->setState(true);
        sceneRenderer->drawScene(*scene);
	}

    /**
     * \brief Undo the last rake.
     * 
     * Reverting only the last step is very cheap by swapping buffer pointers.
     * Calling it again will swap back (e.g. undo the undo).
     * 
     **/
    void EMSCRIPTEN_KEEPALIVE undoLastRake() {
        checkSetup();
        checkState(false,);

        rakeRenderer->swapBuffers();
        redraw();
    }

/*------------------------------------------
Init stuff:
--------------------------------------------*/
    ///< Currently does nothing. Initialization is the frontend's job.
    int EMSCRIPTEN_KEEPALIVE main()
    {
        // keep WASM module alive
        EM_ASM(Module['noExitRuntime'] = true);
        return 0;
    }
}
