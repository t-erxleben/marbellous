#pragma once

#include <string>
#include <emscripten/html5.h>

class WGLContext
{

    private:
        // realize singleton pattern
        static WGLContext* instance;
        WGLContext(std::string canvasID);

        // part of interface to front end
        // may be used to init context at actual canvas
        friend void initWGLContext(std::string canvasID);

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
};