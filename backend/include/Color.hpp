#pragma once

#include <tuple>

class Color
{
private:
    uint8_t r,g,b;

public:

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

    // construct from color code
    Color(unsigned const int);

    // set either via color channels
    void set(uint8_t const r, uint8_t const g, uint8_t const b);

    // set via color code
    void set(unsigned const int);

    // get color channels
    std::tuple<uint8_t, uint8_t, uint8_t> getRGB();
};