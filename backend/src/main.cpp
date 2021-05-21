#include <emscripten.h>
#include <cstdio>
#include <cmath>


/*------------------------------------------
Interface to front end:
--------------------------------------------*/

bool setupDone = false;


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





/*------------------------------------------
Init stuff:
--------------------------------------------*/

int EMSCRIPTEN_KEEPALIVE main()
{
    // setup
    setupDone = true;

    // keep WASM module alive
    EM_ASM(Module['noExitRuntime'] = true);
    return 0;
}

}
