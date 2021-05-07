#pragma once

#include <GLES2/gl2.h>

class Point
{
    public:

        GLfloat x;
        GLfloat y;

        Point(GLfloat x, GLfloat y);

        // translate on torus (over- and underflow)
        void translate(GLfloat tx, GLfloat ty);
    
};