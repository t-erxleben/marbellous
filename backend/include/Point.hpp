#pragma once

#include <GLES3/gl3.h>
#include <math.h>
#include <iostream>

constexpr size_t PRIMITIVE_RESTART = 4294967295;

/**
 * \brief Contains a point in 2D (GLfloat).
 * 
 */
class __attribute__((packed)) Point
{
public:
    GLfloat x; ///< x coordinate
    GLfloat y; ///< y coordinate

    /** 
	 * Construct point at origin.
     **/
    Point(): x{0}, y{0} {};

    /** Construct point by coordinates.
     * \param x X coordinate
     * \param y Y coordinate
     * \return Point Object
     **/
    Point(GLfloat x, GLfloat y) : x{x}, y{y} {};

    /** Translate the Point
     * \param tx Translation along x-axis
     * \param ty Translation along y-axis
     **/
    void translate(GLfloat tx, GLfloat ty)
    {
        this->x += tx;
        this->y += ty;
    };

	/**
	 * \brief Interpolate between two points.
	 * 
	 * \param a Point 1
	 * \param b Point 2
	 * \param m Influence of \p a in result.
	 * \return interpolated Point
	 **/
	static Point mix(const Point& a, const Point& b, float m) {
		return Point(
			a.x * m + b.x * (1.f-m),
			a.y * m + b.y * (1.f-m)
		);
	}

	/**
	 * \brief Check two points for equality.
	 * 
	 * \param oth Other point
	 * \return true Points are equal.
	 * \return false Points are not equal.
	 **/
	bool operator==(const Point& oth) const {
		return x == oth.x && y == oth.y;
	}

	/**
	 * \brief Substract two Point objects.
	 * 
	 * \param rh Other Point
	 * \return Point 
	 **/
	Point operator-(const Point& rh) const {
		return Point(x - rh.x, y - rh.y);
	}

	/**
	 * \brief Add two Point objects.
	 * 
	 * \param rh Other Point.
	 * \return Point 
	 **/
	Point operator+(const Point& rh) const {
		return Point(x + rh.x, y + rh.y);
	}

	/// scale a point.
	/** \param f factor for inverse scaling.
	 * 	\return a new point where res = this / f
	 */
	Point operator/(float f) const {
		return Point(x / f, y / f);
	}

	/// Inner product of two points.
	/* 
	 * \param rh Other Point
	 * \return Result of inner product.
	 **/
	float operator*(const Point& rh) const {
		return x*rh.x + y*rh.y;
	}

	/**
	 * \brief Scale a point.
	 * 
	 * \param f Scaling factor.
	 * \return Reference to result for chaining.
	 **/
	Point& operator*=(float f) {
		x *= f;
		y *= f;
		return *this;
	}

	/**
	 * \brief Same as P*= 1/\p f
	 * 
	 * \param f Scale with 1/f
	 * \return Reference to result for chaining.
	 **/
	Point& operator/=(float f) {
		x /= f;
		y /= f;
		return *this;
	}

	/**
	 * \brief Add two points.
	 * 
	 * \param rh Other Point
	 * \return Reference to result for chaining.
	 **/
	Point& operator+=(const Point& rh) {
		translate(rh.x, rh.y);
		return *this;
	}

	/**
	 * \brief Stream operator for convenient debug output.
	 * 
	 * \param out An std::ostream reference.
	 * \param p Point to output.
	 * \return std::ostream& 
	 **/
	friend std::ostream& operator<<(std::ostream& out, const Point& p) {
		out << "(" << p.x << "," << p.y << ")";
		return out;
	}

};

/**
 * Calculate squarred distance between two Point objects.
 * 
 * \return Squarred distance
 */
inline float distance2(const Point& a, const Point& b) {
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	return dx*dx + dy*dy;
}

/**
 * \brief Displacement information for a Point.
 * 
 * This point will be displaced based on a a new circle at \ref p with radius \ref r. 
 * 
 **/
struct __attribute__((packed)) Displacement {
	Point p;
	float r;
};
