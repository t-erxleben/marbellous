#pragma once

#include <string>
#include <GLES2/gl2.h>

class WGLRenderer
{
    private:
        GLint shaderProgram;

        GLuint compileShader(GLenum const type, std::string const shader) const;
        void setupShaderProgram();

    public:
        WGLRenderer();
        void setActive() const;

};