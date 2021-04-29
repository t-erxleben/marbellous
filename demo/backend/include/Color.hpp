#pragma once

#include <tuple>

class Color
{
private:
    uint8_t r,g,b;

public:

    // default will return black
    Color();

    // construct by giving values for color channels
    Color(uint8_t r, uint8_t g, uint8_t b);

    // construct from color code
    Color(unsigned int);

    // set either via color channels
    void set(uint8_t r, uint8_t g, uint8_t b);

    // set via color code
    void set(unsigned int);

    // get color channels
    std::tuple<uint8_t, uint8_t, uint8_t> getRGB();
};