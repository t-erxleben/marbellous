#include <emscripten.h>
#include <cstdio>
#include <cmath>

#include "Renderer.hpp"
/*------------------------------------------
Interface to front end:
--------------------------------------------*/

bool setupDone = false;
Renderer* re;

// draw a circle at point (x,y) (should be normed to [0,1]^2) with radius r in the given color
void EMSCRIPTEN_KEEPALIVE dropColor(float const x, float const y, float const r, unsigned int const color)
{
    if (setupDone)
    {
        //points on circle, vertex count is arbitrary hard coded for now
        int count = (int)500*r;

        // generate vertex list for triangle fan
        GLfloat* vert = new GLfloat[2*(count+2)];
        vert[0] = x;
        vert[1] = y;

        for(int i = 0; i < count+1; ++i)
        {
            int j = 2*(i+1);
            float angle = (float)i/(float)count * 2 * M_PI;
            vert[j] = r*cosf(angle) + x;
            vert[j+1] = r*sinf(angle) + y;
        }

        re->drawTriangleFan(vert, count+2, Color{color});

        delete[] vert;
    }
    else
    {
        printf("Backend is not initialized yet!");
    }
}

int EMSCRIPTEN_KEEPALIVE main()
{
    // setup
    re = new Renderer{"#myCanvas"};
    setupDone = true;

    dropColor(0, 0, 0.33, 0x0);
    dropColor(0, 0.5, 0.25, 0x00010b);

    // keep WASM module alive
    EM_ASM(Module['noExitRuntime'] = true);
    return 0;
}