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
    Color(): r{0}, g{0}, b{0}, m_ratio{1} {}

    /** Construct color by RGB values.
     * @param r Red channel
     * @param g Green channel
     * @param b Blue channel
     * @return Color Object
    */
	Color(uint8_t const red, uint8_t const green, uint8_t const blue)
		: r{red}, g{green}, b{blue}, m_ratio{1} {}

    
    Color(unsigned const int color) ///< construct from color code
		: Color(color / 256 / 256, (color / 256) % 256, color % 256) {}


    void set(uint8_t const red, uint8_t const green, uint8_t const blue) ///< set via color channels
	{
		r = red;
		g = green;
		b = blue;
	}

    void set(unsigned const int color) ///< set via color code
	{
		r = color / 256 / 256;
		g = (color / 256) % 256;
		b = color % 256;
	}

   	rgb_t getRGB() const ///< get color channels
	{
		return std::make_tuple(r,g,b);
	}
};

