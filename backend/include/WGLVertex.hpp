#pragma once

#include <GLES3/gl3.h>

#include "Point.hpp"

// no further logic needed, therefor only a struct
struct __attribute__ ((packed)) WGLVertex
{
	constexpr WGLVertex (Point p, GLuint z, GLuint color) 
	: p{p}, z{z}, color{color}
	{}
	constexpr WGLVertex() : p{}, z{}, color{} {}
    Point p;
	GLuint z;
    GLuint color; ///< This was of type GLushort first but we would have needed padding bytes.
};
