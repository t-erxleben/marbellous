#pragma once

#include <forward_list>

#include "Point.hpp"

class Polygon
{
    private:

        std::forward_list<Point> vertices;
        
        // Index of color inside a palette
        // exchanging palette can be used to try different color combinations without reassigning color to each polygon
        size_t colorIndex;
    
    public:

        static Polygon createCirlce(Point mid, float radius);

        // TODO some interface to get all information for drawing

        // displace polygon (e.g. new drop is close)
        // arguments not known yet
        void displace();
};