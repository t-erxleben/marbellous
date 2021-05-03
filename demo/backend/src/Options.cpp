#include "Options.hpp"

Options* const Options::getInstance()
{
    if(!instance)
    {
        instance = new Options{};
    }
    return instance;
}

void Options::setPalette(Palette const & newPalette)
{
    activePalette = newPalette;
}

Palette* Options::getPalette()
{
    return &activePalette;
}

void Options::setBGColor(Color const & col)
{
    bgColor = col;
}

Color* Options::getBGColor()
{
    return &bgColor;
}