#pragma once

#include <cstring>
#include <vector>

#include "WGLRenderer.hpp"
#include "Scene.hpp"

class WGLSceneRenderer: private WGLRenderer
{
    private:
        void constructBuffers(void** indices, void** vertices, Scene const & scene, size_t & indices_size, size_t & vertices_size);

        std::string const vertex_source{"attribute vec2 position; void main(){gl_Position = vec4(position, 0.0, 1.0);}\n"};
    
        // todo
        std::string const fragment_source{"precision mediump float; attribute uint colorCode; uniform vec3 c0; uniform vec3 c1; uniform vec3 c2; uniform vec3 c3; vec3 colorMap(in uint colCode, out vec4 color){if} void main(){gl_FragColor = vec4(color, 1.0);}\n"};

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