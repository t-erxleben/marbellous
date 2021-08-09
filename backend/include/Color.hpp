#pragma once

#include <tuple>
#include <cstdint>

class Color
{
private:
    uint8_t r,g,b;
	unsigned m_ratio;
	float m_p;

	friend class Palette;
public:
	using rgb_t = std::tuple<uint8_t, uint8_t, uint8_t>;

	bool operator==(const Color& oth) const {
		return r == oth.r && g == oth.g && b == oth.b;
	}

    /** Construct black.
    */  
    Color();

    /** Construct color by RGB values.
     * @param r Red channel
     * @param g Green channel
     * @param b Blue channel
     * @return Color Object
    */
    Color(uint8_t const r, uint8_t const g, uint8_t const b);

    
    Color(unsigned const int); ///< construct from color code

    void set(uint8_t const r, uint8_t const g, uint8_t const b); ///< set via color channels

    void set(unsigned const int); ///< set via color code

   	rgb_t getRGB() const; ///< get color channels
};
