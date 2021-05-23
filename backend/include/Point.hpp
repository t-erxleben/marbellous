#pragma once

#include <GLES2/gl2.h>
#include <math.h>
#include <earcut.hpp>

/**
 * Contains a point in 2D (GLfloat).
 */
class __attribute__((packed)) Point
{
public:
    GLfloat x;
    GLfloat y;

    /** Construct point by coordinates.
     * @param x X coordinate
     * @param y Y coordinate
     * @return Point Object
    */
    Point(GLfloat x, GLfloat y) : x{x}, y{y} {};

    /** Translate the Point
     * @param tx Translation along x-axis
     * @param ty Translation along y-axis
    */
    void translate(GLfloat tx, GLfloat ty)
    {
        this->x += tx;
        this->y += ty;
    };
};


// Add specialized template function to use our custom point in the earcutting lib
namespace mapbox {
namespace util {

template <>
struct nth<0, Point> {
    inline static auto get(const Point &t) {
        return t.x;
    };
};
template <>
struct nth<1, Point> {
    inline static auto get(const Point &t) {
        return t.y;
    };
};

} 
}