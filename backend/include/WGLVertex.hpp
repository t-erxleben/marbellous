#pragma once

#include <GLES3/gl3.h>

#include "Point.hpp"

// no further logic needed, therefor only a struct
typedef struct __attribute__ ((packed)) WGLVertex
{
    Point p;
    GLuint color; ///< This was of type GLushort first but we would have needed padding bytes.
} WGLVertex;