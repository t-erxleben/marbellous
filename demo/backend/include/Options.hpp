#pragma once

#include "Palette.hpp"

class Options
{
    private:
        // realize singleton pattern
        static Options* instance;
        Options();


        // private data members
        Palette activePalette{};
        Color bgColor{0xFFFFFF};

    public:
        // singleton should neither be copied nor assigned, moved etc.
        Options(Options const &) = delete;
        void operator=(Options const &) = delete;
        Options& operator=(Options&&) = delete;
        Options(Options&&) = delete;

        // access instance
        Options * const getInstance();

        void setPalette(Palette const & newPalette);
        Palette* getPalette();
        void setBGColor(Color const & col);
        Color* getBGColor();


};