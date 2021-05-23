#pragma once

#include <utility>

#include "Palette.hpp"

class Options
{
    private:
        Options();

        static Options* instance; ///< Single options instance.
        std::vector<Palette> Palettes; ///< All saved palettes.
        size_t activePalette; ///< Index of currently active palette inside palettes vector.
        Color bgColor{0xFFFFFF}; ///< Background color.


    public:
        Options(Options const &) = delete;
        void operator=(Options const &) = delete;
        Options& operator=(Options&&) = delete;
        Options(Options&&) = delete;

        /** Return single options instance.
         * If there is no existing instance it will be constructed.
         * @return Options
        */
        static Options * const getInstance();

        // handling Palettes
        size_t addPalette(Palette const & newPalette);
        Palette* getActivePalette();
        int setActivePalette(size_t id);

        // handling BG color
        void setBGColor(Color const & col);
        Color* getBGColor();
};