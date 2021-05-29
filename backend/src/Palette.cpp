#include <Palette.hpp>

Palette::Palette(): colors{}
{}

Palette::Palette(size_t num_colors): colors(num_colors)
{}

size_t Palette::add(Color const col)
{
    colors.push_back(col);
    return colors.size() - 1;
}

Color& Palette::operator[](size_t const index)
{
    return colors.at(index);
}

const Color& Palette::operator[](size_t const index) const
{
    return colors.at(index);
}

size_t Palette::getSize() const
{
    return colors.size();
}
