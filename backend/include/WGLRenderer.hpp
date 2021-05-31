#pragma once

#include <string>
#include <GLES3/gl3.h>
#include <EGL/egl.h>

class WGLRenderer
{
public:
    GLint shaderProgram;
    GLuint compileShader(GLenum const type, std::string const shader) const;
    void setupShaderProgram(std::string vert, std::string frag);
};