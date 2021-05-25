#pragma once

#include <cstring>
#include <vector>

#include "WGLRenderer.hpp"
#include "Scene.hpp"

class WGLSceneRenderer: private WGLRenderer
{
    private:
        void constructBuffers(void** indices, void** vertices, Scene const & scene, size_t & indices_size, size_t & vertices_size);

        std::string const vertex_source{"#version 300 es\nin vec2 position; in uint colorCode; flat out uint colID; void main(){gl_Position = vec4(position, 0.0, 1.0); colID = colorCode;}\n"};
    
        // todo
        std::string const fragment_source{"#version 300 es\nprecision mediump float;\nflat in uint colID;\nuniform vec3 c0;\nuniform vec3 c1;\nuniform vec3 c2;\nuniform vec3 c3;\nout vec4 fFragment;\nvoid colorMap(in uint cc, out vec4 color) {\n    switch(cc) {\n        case 0u:\n            color = vec4(c0, 1.0f);\n            break;\n        case 1u:\n            color = vec4(c1, 1.0f);\n            break;\n        case 2u:\n            color = vec4(c2, 1.0f);\n            break;\n        case 3u:\n            color = vec4(c3, 1.0f);\n            break;\n        default:\n            color = vec4(0.0f, 0.0f, 0.0f, 1.0f);\n    }\n}\nvoid main() {\n    vec4 color = vec4(0.0, 0.0, 0.0, 0.0);\n    colorMap(colID, color);\n    fFragment = color;\n}\n"};

        GLuint vao;
        GLuint ebo;

        GLint color0Loc;
        GLint color1Loc;
        GLint color2Loc;
        GLint color3Loc;
        
    public:
        WGLSceneRenderer();
        void drawScene(Scene const & scene);
        void setActive();
};