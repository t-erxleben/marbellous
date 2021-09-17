#pragma once

#include <utility>

#include "Palette.hpp"

/**
 * \brief Global option state.
 * 
 * \details A singleton representing general options and data of the backend.
 * 
 **/
class Options
{
    private:
        Options(); ///< default constructor
        std::vector<Palette> Palettes; ///< All saved palettes.
        size_t activePalette; ///< Index of currently active palette inside palettes vector.
        Color bgColor; ///< Background color.
        bool dropState; ///< State of the backend. True for drop stzate, false for rake state.
        bool filter; ///< Enable or disable filtering.

    public:
        Options(Options const &) = delete;
        void operator=(Options const &) = delete;
        Options& operator=(Options&&) = delete;
        Options(Options&&) = delete;

        /** Return single options instance.
         * If there is no existing instance it will be constructed.
         * \return Options
         **/
        static Options * const getInstance();

        /**
         * Add a new palette.
         * 
         * \param newPalette Palette to add.
         * 
         * \returns ID of the new palette. May be used to set it active.
         **/
        size_t addPalette(Palette const & newPalette);

        /**
         * Get the active palette.
         * 
         * \return Active palette.
         **/
        Palette* getActivePalette();
		/// const version of getActivePalette()
        const Palette* getActivePalette() const;

        /**
         * Set the active palette.
         * 
         * \param id ID of the palette to set active. Return value of addPalette().
         **/
        int setActivePalette(size_t id);

        /**
         * Set the state of the backend. This influences which API functions are active.
         * 
         * \param dropState TRUE for drop state and FALSE for rake state.
         **/
        void setState(bool dropState);

        /**
         *  Get the backend's state. This influences which API functions are active.
         * 
         * \return TRUE for drop state, FALSE for rake state.
         **/
        bool getState() const;

        /**
         * Set the background color.
         * 
         * \param col Color to use for the background.
         **/
        void setBGColor(Color const & col);

        /**
         * Get the Background color.
         * 
         * \return Pointer to the background color.
         **/
        const Color* getBGColor() const;

        /**
         * Set the option wether to filter or not.
         * 
         * \param filter TRUE for filter application, FALSE for no filter application.
         **/
        void setFilter(bool filter);

        /**
         * \brief Get the filter option
         * 
         * \return true Filter will be applied.
         * \return false No filter will be applied.
         **/
        bool getFilter() const;
};
