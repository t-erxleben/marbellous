#pragma once

#include <vector>

#include "Polygon.hpp"

class Scene
{
    private:

        std::vector<Polygon> polygons;

    public:

        void addPolygon(Polygon const & pol);
        
        // just give vector iterators to enable for each loop for scenes
        std::vector<Polygon>::iterator begin();
        std::vector<Polygon>::iterator end();
};