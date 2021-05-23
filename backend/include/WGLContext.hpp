#pragma once

#include <string>
#include <iostream>
#include <emscripten/html5.h>
#include <GLES2/gl2.h>

#include "Options.hpp"

using CanvasSize = std::pair<size_t, size_t>;

class WGLContext
{

    private:
        // realize singleton pattern
        static WGLContext* instance;
        WGLContext(std::string canvasID, size_t x, size_t y);

        // part of interface to front end
        // may be used to init context at actual canvas
        friend void initWGLContext(char* canvasID);

        CanvasSize canvasSize; ///< Canvas size in pixels.

        EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context;

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

        void setCanvasSize(size_t x, size_t y);
        CanvasSize getCanvasSize();
};