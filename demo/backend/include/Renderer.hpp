#pragma once

#include <string>
#include <vector>
#include <emscripten.h>
#include <emscripten/html5.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>

#include "Color.hpp"

class Renderer
{
private:
    GLint shaderProgram;
    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context;

    GLuint compileShader(GLenum type, std::string shader);
    void setupContext(std::string canvasID);
    void setupShaderProgram();

public:
    Renderer(std::string canvasID);
    void drawTriangleFan(GLfloat* points, size_t pointCount, Color col);


};