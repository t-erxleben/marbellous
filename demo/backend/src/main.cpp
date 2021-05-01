#include <emscripten.h>
#include <cstdio>

/*------------------------------------------
Interface to front end:
--------------------------------------------*/

// draw a circle at point (x,y) (should be normed to [0,1]^2) with radius r in the given color
void EMSCRIPTEN_KEEPALIVE dropColor(double const x, double const y, double const r, unsigned int const color)
{
    printf("Color drop at (%f, %f) with radius = %f and color #%X", x, y, r, color);
}

int main()
{
    // TODO: Init WebGL object and stuff
    return 0;
}