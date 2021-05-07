#include <emscripten.h>
#include <cstdio>
#include <cmath>


/*------------------------------------------
Interface to front end:
--------------------------------------------*/

bool setupDone = false;

// draw a circle at point (x,y) (should be normed to [0,1]^2) with radius r in the given color
extern "C" {
int EMSCRIPTEN_KEEPALIVE addDrop(float const x, float const y, float const r, unsigned int const color)
{
    // TODO
    if (setupDone)
    {
        
    }
    else
    {
        printf("Backend is not initialized yet!");
    }
    return 0;
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
