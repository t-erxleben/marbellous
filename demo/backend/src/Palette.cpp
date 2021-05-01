#include <Palette.hpp>

size_t const Palette::add(Color const col)
{
    colors.push_back(col);
    return colors.size() - 1;
}

Color& Palette::operator[](size_t const index)
{
    return colors.at(index);
}