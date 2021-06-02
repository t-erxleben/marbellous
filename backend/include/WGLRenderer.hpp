#pragma once

#include <string>
#include <GLES3/gl3.h>
#include <EGL/egl.h>

class WGLRenderer
{
public:
    GLuint compileShader(GLenum const type, std::string const shader) const;
    GLuint setupProgram(std::string vert, std::string frag);
};
