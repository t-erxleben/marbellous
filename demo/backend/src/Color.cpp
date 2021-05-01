#include "Color.hpp"

Color::Color(): r{0}, g{0}, b{0}
{}

Color::Color(uint8_t const red, uint8_t const green, uint8_t const blue): r{red}, g{green}, b{blue}
{}

Color::Color(unsigned int const color)
{
    Color(color / 256 / 256, (color / 256) % 256, color % 256);
}

void Color::set(uint8_t const red, uint8_t const green, uint8_t const blue)
{
    r = red;
    g= green;
    b = blue;
}

void Color::set(unsigned int const color)
{
    r = color / 256 / 256;
    g = (color / 256) % 256;
    b = color % 256;
}

std::tuple<uint8_t, uint8_t, uint8_t> Color::getRGB()
{
    return std::tuple<uint8_t, uint8_t, uint8_t>{r,g,b};
}