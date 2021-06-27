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

    /**
     * @return Begin iterator over colors
    */
    std::vector<Color>::iterator begin()
    {
        return colors.begin();
    }

    /**
     * @return Begin const_iterator over colors
    */
    std::vector<Color>::const_iterator begin() const
    {
        return colors.begin();
    }

    /**
     * @return End iterator over colors
    */
    std::vector<Color>::iterator end()
    {
        return colors.end();
    }

    /**
     * @return End const_iterator over colors
    */
    std::vector<Color>::const_iterator end() const
    {
        return colors.end();
    }

};
