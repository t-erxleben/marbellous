#pragma once

#include <vector>

#include "Point.hpp"
#include "WGLVertex.hpp"
#include "WGLContext.hpp"

/**
 * Represent polygons in 2D as a list of vertices in counter-clockwise order.
 */
class Polygon
{
private:
	static constexpr float MIN_DISTANCE = 0.9f; ///< minimal distance between vertices in pixel
	static constexpr float MAX_DISTANCE = 2.f; ///< maximal distance between vertices in pixel
	struct Data {
		Point pos;
		int insertion; ///< >0 -> insert after this vertex x, <0 remove this
		Point last;
	};
    std::vector<Data> vertices; ///<List representation of the polygon.

    GLuint colorIndex; ///< Index of a color inside a palette. Exchanging the active palette will change the drawing color.
    
    bool isCircle;
    Point creationPoint;

    /** Compute the number of vertices needed for a "circle" to appear smooth.
     * Canvas size is loaded from the options.
     * 
     * Math explanation: 
     * The distance between two vertices should be close to the distance between two pixels
     * The canvas is defined within [-1,1], meaning that its length is 2 
     * 
     * It follows that (circumference/count) (the length of the arc between two vertices, 
     * for short distances approx. a line), should be equal to 2/canvaslengthpx
     * (which computes the distance between pixels in [-1,1] canvas space)
     * -> circumference/count = 2/canvaslengthpx
     *
     * now that we have a value for (circumference/count) as well as the radius, we can calculate: count = circumf / (circumf/count)
     * with cirumf = 2 * pi * radius
     * we get count = (2*pi*radius)/(2/canvaslengthpx)
     * we can then simplify it by canceling the 2s
     * -> count = (pi*radius)/(1/canvaslengthpx)
     * and then we notice we divide by 1/canvaslengthpx which is the same as multiplying, thus we arrive at
     * -> count = pi*radius*canvaslengthpx
     * 
     * \param radius Radius of the circle.
     * \return Number of vertices.
     * @attention If we decide to support non square canvasses this needs to be changed as well.
    */
    static size_t circleVertCount(float radius);
    
	/** calculate how many points should be inserted between two
	 * adjunctive points
	 * @param p0,p1 points to calc insertion between
	 * @param min_dis2,max_dis2 square of minimal/maximal distance between vertices
	 */
	static int calcInsertion(const Point& p0, const Point& p1, float min_dis2, float max_dis2);

public:

	static constexpr float MIN_R = 0.0001;

    /** Create an empty Polygon.
    */
    Polygon();

    /** Create a circle.
     * \param mid Middle point of the circle.
     * \param radius Radius of the circle.
    */
    Polygon(Point mid, float radius, GLuint colorIndex);

    /**
     * \return Number of vertices of a polygon.
    */
    size_t getVertCount() const;

    /** Fill vectors with infos needed for drawing. 
     * Intended to be used for glDrawElements(). Therefore 3 consecutive indices define one triangle to be drawn
     * and refer to vertices inside the vertices vector.
     * \param indices Vector to be filled with triangle information.
     * \param vertices Vector to be filled with vertices.
     * @attention Winding order is clockwise so face culling in WGL needs to be disabled. 
     * This is no performance limitation as each triangle is allways visible.
    */
    void getDrawInfo(std::vector<WGLVertex>& vertices, GLuint z) const;

    /** Return the point which was the original middle point.
     * \return middle point
    */
    Point const & getCreationPoint();

    /** Displace a polygon as a result of a new circle appearing.
	 * \param displacements vector of circles with radius and diameter which causes displacement
	 * \param canvasSize size of canvas in pixel to calculate vertex insertion for smooth edges.
    */
    void displace(const std::vector<Displacement>& displacements, size_t canvasSize);

    /** Redraw Polygon as a new circle.
     * This is intended to be used for making the circle larger.
     * \param mid Middle point of the circle.
     * \param radius Radius of the circle.
     * @attention Will throw an error if used on a polygon that is no longer a circle.
    */
    void makeCircle(Point mid, float radius);
	
};
