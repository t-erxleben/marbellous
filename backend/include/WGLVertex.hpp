#pragma once

#include <GLES3/gl3.h>

#include "Point.hpp"

/**
 * \brief Struct representing one vertex.
 * 
 * Aligned with memory so that an array of WGLVertex structs can be send directly to GPU.
 * 
 **/
struct __attribute__ ((packed)) WGLVertex
{
    Point p; ///< Actual vertex point.
	GLuint z; ///< Artificial depth value of the point used for the drawing algorithm.
    GLuint color; ///< This was of type GLushort first but we would have needed padding bytes.
};
