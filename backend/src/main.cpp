#include <emscripten.h>
#include <cstdio>
#include <cmath>

#include "WGLContext.hpp"
#include "WGLSceneRenderer.hpp"
#include "Scene.hpp"
#include "Options.hpp"

#define checkSetup(RET) if (!setupDone) \
        { \
            fprintf(stderr, "Backend is not initialized yet!"); \
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
    }

    // draw a circle at point (x,y) (should be normed to [0,1]^2) with radius r in the given color
    int EMSCRIPTEN_KEEPALIVE addDrop(float const x, float const y, float r, unsigned int const color)
    {
        checkSetup(-1);

        // catch degenerated circle (earcut would fail to tesselate it)
        r = (r>=0.001)?r:0.001;
        int handle = scene->addPolygon(Polygon{Point{x,y}, r, color});
        sceneRenderer->drawScene(*scene);
        return handle;
    }

    // resize drop of given ID (return val of addDrop(...))
    // influence of resizeVal is not defined yet
    int EMSCRIPTEN_KEEPALIVE resizeDrop(int const dropID, float const newRadius)
    {
        checkSetup(-1);
        try
        {
            auto &p = (*scene)[dropID];
            p.makeCircle(p.getCreationPoint(), newRadius);
        }
        catch(const std::exception& e)
        {
            fprintf(stderr, "Tried to resize non existing drop!\n");
            return -1;
        }
        sceneRenderer->drawScene(*scene);
    
        return 0;
    }

    char *EMSCRIPTEN_KEEPALIVE getImage()
    {
        checkSetup(NULL);
        constexpr char prefix[] = "P6\n720\n720\n255\n";
        constexpr int prefix_len = sizeof(prefix) - 1;
        static std::vector<char> data(720 * 720 * 4 + prefix_len + 1);
        data[0] = prefix_len;
        memcpy(data.data() + 1, prefix, prefix_len);
        sceneRenderer->drawToBuffer(*scene, data.data() + prefix_len + 1, static_cast<int>(data.size() - prefix_len - 1));
        char *ptr = data.data() + prefix_len + 1;
        for (int i = 0; i < 720 * 720; ++i)
        {
            ptr[3 * i] = ptr[4 * i];
            ptr[3 * i + 1] = ptr[4 * i + 1];
            ptr[3 * i + 2] = ptr[4 * i + 2];
        }
        return data.data();
    }
  
    // deprecated and only draws in color 0
    void EMSCRIPTEN_KEEPALIVE dropColor(float const x, float const y, float const r, unsigned int const color)
    {
        checkSetup();
        if (r > 0.01) // TODO: replace with eps
        {
            scene->addPolygon(Polygon{Point{x, y}, r, 0});
            sceneRenderer->drawScene(*scene);
        }
    }

/*------------------------------------------
Init stuff:
--------------------------------------------*/

    int EMSCRIPTEN_KEEPALIVE main()
    {
        // setup

        ///This needs to be done in the front end (init call should be after bg):
        char id[] = "#image";
        setBGColor(0xf7e9ce);
        setActivePalette(addPalette(2));
        setPaletteColors(0x33CC33, 0x0000CC, 0xFFCC00, 0xFF0000);
        initWGLContext(id, 720, 720);
        ///--------------------------------

        sceneRenderer = new WGLSceneRenderer{};
        scene = new Scene{};

        setupDone = true;

        //int drop = addDrop(0, 0, 0.5, 2);
        //resizeDrop(drop, 0.2);

        // keep WASM module alive
        EM_ASM(Module['noExitRuntime'] = true);
        return 0;
    }
}
