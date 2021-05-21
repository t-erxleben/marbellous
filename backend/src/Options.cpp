#include "Options.hpp"

Options* const Options::getInstance()
{
    if(!instance)
    {
        instance = new Options{};
    }
    return instance;
}

size_t Options::addPalette(Palette const & newPalette)
{
    Palettes.push_back(newPalette);
    return Palettes.size() - 1;
}

Palette* Options::getActivePalette()
{
    return &Palettes[activePalette];
}

int Options::setActivePalette(size_t id)
{
    if(id >= Palettes.size())
    {
        return -1;
    }
    activePalette = id;
    return 0;
    
}

void Options::setBGColor(Color const & col)
{
    bgColor = col;
}

Color* Options::getBGColor()
{
    return &bgColor;
}

void Options::setCanvasSize(size_t x, size_t y)
{
    canvasSize.first = x;
    canvasSize.second = y;
}


CanvasSize Options::getCanvasSize()
{
    return canvasSize;
}