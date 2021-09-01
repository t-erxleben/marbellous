#pragma once

#include <vector>
#include <cstddef>

#include "Color.hpp"

/**
 * \brief Contains a vector of colors.
 * \addtogroup util
 */
class Palette
{
private:
    
    std::vector<Color> colors{}; ///< internal palette representation

    /**
     * \brief Update palette drop probability based on ratios.
     * \internal
     **/
	void updateP();

public:

    /**
     * \brief Construct a new Palette object
     * 
     **/
    Palette();

    /**
     * \brief Construct a new Palette object with \p colors colors.
     * 
     * \param colors Number of colors to pre allocate.
     **/
    Palette(size_t colors);

    /**
     * \brief Add a color to the palette.
     * 
     * \param col Color object to add.
     * \return ID of the color inside the Palette. May be used to modify it later.
     **/
    size_t add(Color const col);

    /**
     * \brief Get the palette's size.
     * 
     * \return Size of the palette.
     **/
    size_t getSize() const;

    /**
     * \brief Get or set the color at \p index.
     * 
     * \attention Might throw std::out_of_range for non existing colors.
     * 
     * \param index Color to change.
     * \return Reference to the color at \p index.
     **/
    Color& operator[](size_t const index); ///< get or set the color at index, might throw std::out_of_range

    /**
     * \brief Get the color at \p index.
     * 
     * \attention Might throw std::out_of_range for non existing colors.
     * 
     * \param index Color to get.
     * \return Constant reference to the color at \p index.
     **/
    const Color& operator[](size_t const index) const;

    /**
     * \brief Returns a random color of that pallet based on the ratios.
     * 
     * \return ID of a random color. 
     **/
	unsigned getRandomColorId() const;

    /**
     * \brief Set the ratio of the color at \p index.
     * 
     * \param index ID of color to change the ratio for.
     * \param ratio New ratio.
     **/
	void setRatioAt(size_t index, unsigned ratio); ///< updates a ratio
	void setColorAt(size_t index, const Color& color); ///< set new color and update ratios


    /**
     * \return Begin const_iterator over colors
    */
    std::vector<Color>::const_iterator begin() const
    {
        return colors.begin();
    }

    /**
     * \return End const_iterator over colors
    */
    std::vector<Color>::const_iterator end() const
    {
        return colors.end();
    }

};
