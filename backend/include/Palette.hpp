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

	void updateP();

public:
    Palette();
    Palette(size_t colors);

    size_t add(Color const col); ///< adds color to palette and returns its index

    size_t getSize() const;

    Color& operator[](size_t const index); ///< get or set the color at index, might throw std::out_of_range
    const Color& operator[](size_t const index) const;

	unsigned getRandomColorId() const; ///< returns a random color of that pallet based on the ratios
	void setRatioAt(size_t index, unsigned ratio); ///< updates a ratio
	void setColorAt(size_t index, const Color& color); ///< set new color and update ratios


    /**
     * @return Begin const_iterator over colors
    */
    std::vector<Color>::const_iterator begin() const
    {
        return colors.begin();
    }

    /**
     * @return End const_iterator over colors
    */
    std::vector<Color>::const_iterator end() const
    {
        return colors.end();
    }

};
