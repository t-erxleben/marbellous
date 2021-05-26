#include <emscripten.h>
#include <cstdio>
#include <cmath>

#include "WGLContext.hpp"
#include "WGLSceneRenderer.hpp"
#include "Scene.hpp"
#include "Options.hpp"

/*------------------------------------------
Interface to front end:
--------------------------------------------*/

bool setupDone = false;
WGLSceneRenderer *sceneRenderer;
Scene *scene;

void _initWGLContext(char* canvasID, size_t x, size_t y)
{
    WGLContext::instance = new WGLContext(canvasID, x, y);
}


extern "C" {
// draw a circle at point (x,y) (should be normed to [0,1]^2) with radius r in the given color
int EMSCRIPTEN_KEEPALIVE addDrop(float const x, float const y, float const r, unsigned int const color)
{
    // TODO
    if (!setupDone)
    {
        fprintf(stderr, "Backend is not initialized yet!");
        return -1;
    }
    
    return 0;
}

// resize drop of given ID (return val of addDrop(...))
// influence of resizeVal is not defined yet
void EMSCRIPTEN_KEEPALIVE resizeDrop(size_t const dropID, float const resizeVal)
{
    if (!setupDone)
    {
        fprintf(stderr, "Backend is not initialized yet!");
        return;
    }
    // TODO
}

int EMSCRIPTEN_KEEPALIVE addPalette()
{
    if (!setupDone)
    {
        fprintf(stderr, "Backend is not initialized yet!");
        return 0;
    }
    // TODO
    return 0;
}

int EMSCRIPTEN_KEEPALIVE setActivePalette(int const id)
{
    if (!setupDone)
    {
        fprintf(stderr, "Backend is not initialized yet!");
        return 0;
    }
    // TODO
    return 0;
}

int EMSCRIPTEN_KEEPALIVE setPaletteColors(unsigned int const c0, unsigned int const c1, unsigned int const c2, unsigned int const c3)
{
    if (!setupDone)
    {
        fprintf(stderr, "Backend is not initialized yet!");
        return 0;
    }
    // TODO
    return 0;
}

void EMSCRIPTEN_KEEPALIVE setSinglePaletteColor(size_t const colorNumber, unsigned int const color)
{
    if (!setupDone)
    {
        fprintf(stderr, "Backend is not initialized yet!");
        return;
    } 
}

void EMSCRIPTEN_KEEPALIVE setBGColor(unsigned int const color)
{
    if (!setupDone)
    {
        fprintf(stderr, "Backend is not initialized yet!");
        return;
    } 
}

void EMSCRIPTEN_KEEPALIVE setCanvasSize(size_t x, size_t y)
{
    if (!setupDone)
    {
        fprintf(stderr, "Backend is not initialized yet!");
        return;
    } 
}

// part of interface to front end
// may be used to init context at actual canvas
void initWGLContext(char* canvasID, size_t x, size_t y)
{
    _initWGLContext(canvasID, x, y);
}

void EMSCRIPTEN_KEEPALIVE dropColor(float const x, float const y, float const r, unsigned int const color)
{
    if (!setupDone)
    {
        fprintf(stderr, "Backend is not initialized yet!");
        return;
    } 
    if(r > 0.01) // TODO: replace with eps
    {
        scene->addPolygon(Polygon{Point{x,y}, r, 0});
        sceneRenderer->drawScene(*scene);
    }
}



/*------------------------------------------
Init stuff:
--------------------------------------------*/

int EMSCRIPTEN_KEEPALIVE main()
{
    // setup
    char id[] = "#image";
    auto opt = Options::getInstance();
    Palette p{};
    opt->setBGColor(Color{0xf7e9ce});
    opt->setActivePalette(opt->addPalette(p));
    opt->getActivePalette()->add(Color{0x33CC33});
    opt->getActivePalette()->add(Color{0x0000CC});
    opt->getActivePalette()->add(Color{0xFFFF00});
    opt->getActivePalette()->add(Color{0xFF0000});
    initWGLContext(id, 720, 720);

    sceneRenderer = new WGLSceneRenderer{};
    scene = new Scene{};

    // Test
    int pol = scene->addPolygon(Polygon{Point{0,0}, 0.5, 0});
    sceneRenderer->drawScene(*scene);
    //auto (*scene)[pol].
    //for()
    //

    setupDone = true;

    // keep WASM module alive
    EM_ASM(Module['noExitRuntime'] = true);
    return 0;
}

}
