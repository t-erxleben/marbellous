#pragma once

#include <vector>

#include "Color.hpp"

/**
 * Contains a vector of colors.
 */
class Palette
{
private:
    
    std::vector<Color> colors{};

public:

    // adds color to palette and returns its index
    size_t add(Color const col);

    // get or set the color at index
    // might throw std::out_of_range
    Color& operator[](size_t const index);
};