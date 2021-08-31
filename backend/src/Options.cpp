#include "Options.hpp"
#include <cstdio>

Options::Options(): bgColor{0xf7e9ce}, dropState{true} , activePalette{0}, Palettes(0), filter{false}
{ }

Options* const Options::getInstance()
{
    static Options instance;
    return &instance;
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

void Options::setState(bool dropState)
{
    this->dropState = dropState;
}

bool Options::getState()
{
    return dropState;
}

void Options::setFilter(bool filter)
{
    this->filter = filter;
}

bool Options::getFilter()
{
    return filter;
}
