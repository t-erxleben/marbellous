#include "WGLContext.hpp"

WGLContext* WGLContext::instance = nullptr;

WGLContext::WGLContext(std::string canvasID, size_t x, size_t y): canvasSize{x,y}
{
    printf("called constructor of context\n");
    emscripten_set_canvas_element_size(canvasID.c_str(), x, y);
    EmscriptenWebGLContextAttributes attrs;
    emscripten_webgl_init_context_attributes(&attrs);

    attrs.explicitSwapControl = 0;
    attrs.depth = 1;
    attrs.stencil = 1;
    attrs.antialias = 1;
    attrs.majorVersion = 2;
    attrs.minorVersion = 0;

    context = emscripten_webgl_create_context(canvasID.c_str(), &attrs);

    emscripten_webgl_make_context_current(context);

    glViewport(0,0, x, y);

    auto bg = Options::getInstance()->getBGColor()->getRGB();
    GLfloat r = ((float)std::get<0>(bg))/255.0f;
    GLfloat g = ((float)std::get<1>(bg))/255.0f;
    GLfloat b = ((float)std::get<2>(bg))/255.0f;
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}


void WGLContext::setCanvasSize(size_t x, size_t y)
{
    canvasSize.first = x;
    canvasSize.second = y;
}


CanvasSize WGLContext::getCanvasSize()
{
    return canvasSize;
}

WGLContext * const WGLContext::getContext()
{
    if(!instance)
    {
        std::cerr << "Someone tried to get an uninitialized WGLContext!" << std::endl;
        return NULL;
    }
    else
    {
        return instance;
    }
}