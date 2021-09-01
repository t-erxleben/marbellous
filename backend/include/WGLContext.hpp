#pragma once

#include <string>
#include <iostream>
#include <emscripten/html5.h>
#include <GLES3/gl3.h>

#include "Options.hpp"

/**
 * \brief Singleton representing the visible WebGL Context.
 * 
 * Needs to be initialized on start up by initBackend() function.
 * Directly refers to the html5 canvas in the front end.
 * Usage of a html5 canvas is the only assumption on the frontend.
 * 
 * \attention Currently dropRes and rakeRes need to be the same.
 * 
 **/
class WGLContext
{

    private:
        static WGLContext* instance; ///< single instance
        size_t dropRes; ///< Canvas size in pixels for drop state.
        size_t rakeRes; ///< Canvas Size in pixels for rake state
        std::string canvasID; ///< CSS identifier of the canvas, needed for resize
        EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context; ///< WGL context handle

        /**
         * \brief Construct a new WGLContext object
         * 
         * \param canvasID CSS identifier of a html5 canvas
         * \param dropRes desired resolution.
         **/
        WGLContext(std::string canvasID, size_t dropRes);
       
        // part of interface to front end (will be called by interface function since extern C functions can not have friends :-( )
        // may be used to init context at actual canvas
        ///< Friend function to use for initialization.
        friend void _initWGLContext(const char* canvasID, size_t x);

        /**
         * \brief Set the canvas clear color to \p bg.
         * 
         * \param bg New clear color.
         **/
		void setBGColor(const Color::rgb_t& bg);

    public:
        WGLContext() = delete;
        WGLContext(WGLContext const &) = delete;
        void operator=(WGLContext const &) = delete;
        WGLContext& operator=(WGLContext&&) = delete;
        WGLContext(WGLContext&&) = delete;

        /**
         * \brief get the current context.
         * 
         * \return Context pointer or NULL if not initialized.
         **/
        static WGLContext * const getContext();

        /**
         * \brief Set the resolution for drop state.
         * 
         * \param x Side lenth in pixels.
         **/
        void setDropRes(size_t x);

        /**
         * \brief Get the drop state resolution.
         * 
         * \return Side lenth in pixels.
         **/
        size_t getDropRes();

        /**
         * \brief Set the resolution for drop state.
         * 
         * \param x Side lenth in pixels.
         **/
        void setRakeRes(size_t x);

        /**
         * \brief Get the drop state resolution.
         * 
         * \return Side lenth in pixels.
         **/
        size_t getRakeRes();

        /**
         * \brief Update the BG color using Options singleton.
         **/
		void updateBGColor();
};
