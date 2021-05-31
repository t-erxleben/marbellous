#pragma once

#include <vector>
#include <cstddef>

#include "Color.hpp"

/**
 * Contains a vector of colors.
 */
class Palette
{
private:
    
    std::vector<Color> colors{};

public:
    Palette();
    Palette(size_t colors);

    // adds color to palette and returns its index
    size_t add(Color const col);

    size_t getSize() const;

    // get or set the color at index
    // might throw std::out_of_range
    Color& operator[](size_t const index);
    const Color& operator[](size_t const index) const;
};
