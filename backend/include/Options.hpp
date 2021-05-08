#pragma once

#include "Palette.hpp"

class Options
{
    private:
        // realize singleton pattern
        static Options* instance;
        Options();


        // private data members
        std::vector<Palette> Palettes;
        size_t activePalette; // index inside Palettes vector
        Color bgColor{0xFFFFFF};

    public:
        // singleton should neither be copied nor assigned, moved etc.
        Options(Options const &) = delete;
        void operator=(Options const &) = delete;
        Options& operator=(Options&&) = delete;
        Options(Options&&) = delete;

        // access instance
        static Options * const getInstance();

        // handling Palettes
        size_t addPalette(Palette const & newPalette);
        Palette* getActivePalette();
        int setActivePalette(size_t id);

        // handling BG color
        void setBGColor(Color const & col);
        Color* getBGColor();


};