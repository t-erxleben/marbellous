#include "WGLContext.hpp"

WGLContext* WGLContext::instance = nullptr;

WGLContext::WGLContext(std::string canvasID, size_t x): dropRes{x}
{
    emscripten_set_canvas_element_size(canvasID.c_str(), x, x);
    this->canvasID = canvasID;
    EmscriptenWebGLContextAttributes attrs;
    emscripten_webgl_init_context_attributes(&attrs);

    attrs.explicitSwapControl = 0; 	// browser decide when to swap -> higher performance
    attrs.depth = 1;				// more polygone magic
    attrs.stencil = 1; 				// needed for polygone magic ^^
    attrs.antialias = 1;			// we want to calculate high quality images so no need for this
    attrs.majorVersion = 2;
    attrs.minorVersion = 0;

    context = emscripten_webgl_create_context(canvasID.c_str(), &attrs);

    emscripten_webgl_make_context_current(context);

    glViewport(0,0, x, x);

	updateBGColor();
    glClear(GL_COLOR_BUFFER_BIT);
}

void WGLContext::setBGColor(const Color::rgb_t& bg) {
    GLfloat r = ((float)std::get<0>(bg))/255.0f;
    GLfloat g = ((float)std::get<1>(bg))/255.0f;
    GLfloat b = ((float)std::get<2>(bg))/255.0f;
    glClearColor(r, g, b, 1.0f);
}

void WGLContext::updateBGColor() {
	setBGColor(Options::getInstance()->getBGColor()->getRGB());
}


void WGLContext::setDropRes(size_t x)
{
    dropRes = x;
    // todo modify screenshot buffer
    emscripten_set_canvas_element_size(canvasID.c_str(), x, x);
}


size_t WGLContext::getDropRes()
{
    return dropRes;
}

void WGLContext::setRakeRes(size_t x)
{
    // todo implement for raking
    dropRes = x;
    emscripten_set_canvas_element_size(canvasID.c_str(), x, x);
    // todo modify screenshot buffer, fbo, textures, ...
}


size_t WGLContext::getRakeRes()
{
    return dropRes;
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

void WGLContext::canvasResize(size_t x)
{
    emscripten_set_canvas_element_size(canvasID.c_str(), x, x);
}

