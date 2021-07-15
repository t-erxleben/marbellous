#pragma once

#include <string>
#include <iostream>
#include <emscripten/html5.h>
#include <GLES3/gl3.h>

#include "Options.hpp"

class WGLContext
{

    private:
        // realize singleton pattern
        static WGLContext* instance;
        size_t dropRes; ///< Canvas size in pixels.
        std::string canvasID; ///< CSS identifier of the canvas, needed for resize
        EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context;
        WGLContext(std::string canvasID, size_t dropRes);
       
        // part of interface to front end (will be called by interface function since extern C functions can not have friends :-( )
        // may be used to init context at actual canvas
        friend void _initWGLContext(const char* canvasID, size_t x);

    public:
        // Context object without actual context would be quite useless
        WGLContext() = delete;
        // Context should neither be copied nor assigned, moved etc.
        WGLContext(WGLContext const &) = delete;
        void operator=(WGLContext const &) = delete;
        WGLContext& operator=(WGLContext&&) = delete;
        WGLContext(WGLContext&&) = delete;

        // access instance if initialized by front end, if not throw exception
        static WGLContext * const getContext();

        void setDropRes(size_t x);
        size_t getDropRes();

        void setRakeRes(size_t x);
        size_t getRakeRes();

		void updateBGColor();
		void setBGColor(const Color::rgb_t& bg);

        void canvasResize(size_t x);
};
