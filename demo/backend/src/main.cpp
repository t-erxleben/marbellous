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
        printf("Color drop at (%f, %f) with radius = %f and color #%X", x, y, r, color);

        //points on circle, vertex count is arbitrary hard coded for now
        int count = (int)500*r;

        // generate vertex list for triangle fan
        GLfloat* vert = new GLfloat[2*count];
        vert[0] = x;
        vert[1] = y;

        for(int i = 2; i < 2*count; i = i + 2)
        {
            float angle = (i / count) * 3.14159;
            GLfloat xi = cos(angle) + x;
            GLfloat yi = sin(angle) + y;
            vert[i] = xi;
            vert[i+1] = yi;
        }
        re->drawTriangleFan(vert, 2*count, Color{color});
        
    }
    else
    {
        printf("Backend is not initialized yet!");
    }
}

int main()
{
    // setup
    re = new Renderer{"canvas ID place holder"};
    setupDone = true;

    // keep WASM module alive
    EM_ASM(Module['noExitRuntime'] = true);
    return 0;
}