#pragma once

#include <GLES2/gl2.h>

#include "Point.hpp"

// no further logic needed, therefor only a struct
typedef struct __attribute__ ((packed)) WGLVertex
{
    Point p;
    GLushort color;
} WGLVertex;