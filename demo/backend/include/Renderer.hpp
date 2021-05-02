#pragma once

#include <string>
#include <vector>
#include <GLES2/gl2.h>

#include "Color.hpp"

class Renderer
{
private:
    GLint shaderProgram;

    GLuint compileShader(std::string shader);
    void setupContext(std::string canvasID);
    void setupShaderProgram();

public:
    Renderer(std::string canvasID);
    void drawTriangleFan(std::vector<std::tuple<double,double>> points, Color col);


};