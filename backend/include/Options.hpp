#pragma once

#include <utility>

#include "Palette.hpp"

class Options
{
    private:
        Options();
        std::vector<Palette> Palettes; ///< All saved palettes.
        size_t activePalette; ///< Index of currently active palette inside palettes vector.
        Color bgColor; ///< Background color.
        bool dropState;
        bool filter;

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
        const Palette* getActivePalette() const;
        int setActivePalette(size_t id);

        void setState(bool dropState);
        bool getState() const;

        void setBGColor(Color const & col);
        const Color* getBGColor() const;

        void setFilter(bool filter);
        bool getFilter() const;
};
