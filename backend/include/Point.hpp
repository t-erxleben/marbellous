#pragma once

#include <GLES3/gl3.h>
#include <math.h>
#include <iostream>

constexpr size_t PRIMITIVE_RESTART = 4294967295;

/**
 * Contains a point in 2D (GLfloat).
 */
class __attribute__((packed)) Point
{
public:
    GLfloat x;
    GLfloat y;

    /** Construct point at origin.
    */
    Point(): x{0}, y{0} {};

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

	static Point mix(const Point& a, const Point& b, float m) {
		return Point(
			a.x * m + b.x * (1.f-m),
			a.y * m + b.y * (1.f-m)
		);
	}

	bool operator==(const Point& oth) const {
		return x == oth.x && y == oth.y;
	}

	Point operator-(const Point& rh) const {
		return Point(x - rh.x, y - rh.y);
	}
	Point operator+(const Point& rh) const {
		return Point(x + rh.x, y + rh.y);
	}

	float operator*(const Point& rh) const {
		return x*rh.x + y*rh.y;
	}

	Point& operator*=(float f) {
		x *= f;
		y *= f;
		return *this;
	}

	Point& operator/=(float f) {
		x /= f;
		y /= f;
		return *this;
	}

	Point& operator+=(const Point& rh) {
		translate(rh.x, rh.y);
		return *this;
	}

	friend std::ostream& operator<<(std::ostream& out, const Point& p) {
		out << "(" << p.x << "," << p.y << ")";
		return out;
	}

};

/**
 * Calculate distance^2 between the points
 */
inline float distance2(const Point& a, const Point& b) {
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	return dx*dx + dy*dy;
}

struct __attribute__((packed)) Displacement {
	Point p;
	float r;
};


/*// Add specialized template function to use our custom point in the earcutting lib
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
}*/
