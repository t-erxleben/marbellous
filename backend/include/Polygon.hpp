#pragma once

#include <forward_list>

#include "Point.hpp"
#include "WGLVertex.hpp"

class Polygon
{
    private:

        std::forward_list<Point> vertices;
        
        // Index of color inside a palette
        // exchanging palette can be used to try different color combinations without reassigning color to each polygon
        size_t colorIndex;
    
    public:

        
        static Polygon createCircle(Point mid, float radius);

        // return the number of vertices
        size_t getVertCount();

        // Fill vectors with infos needed for drawing
        // &vec[0] should give the base pointer for the buffer
        // intended to be used for glDrawElements
        void getDrawInfo(std::vector<GLuint>& indices, std::vector<WGLVertex>& vertices);

        // displace polygon (e.g. new drop is close)
        // arguments not known yet
        void displace();
};