#pragma once

#include <vector>

#include "Polygon.hpp"

class Scene
{
    private:

        std::vector<Polygon> polygons;

    public:

        int addPolygon(Polygon const & pol);

        // get or set the ploygon at index
        // might throw std::out_of_range
        Polygon& operator[](size_t const index);

        // return number of polygons inside the scene
        size_t getPolygonCount();
        
        // just give vector iterators to enable for each loop for scenes
        std::vector<Polygon>::iterator begin();
        std::vector<Polygon>::iterator end();
};