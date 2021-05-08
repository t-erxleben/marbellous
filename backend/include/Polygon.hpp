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

        // Return tuple of vertex indizes and interleaved vertex data (position + color index)
        // intended to be used for glDrawElements
        std::tuple<std::vector<uint32_t> const, size_t, void const *> getDrawInfo();

        // displace polygon (e.g. new drop is close)
        // arguments not known yet
        void displace();
};