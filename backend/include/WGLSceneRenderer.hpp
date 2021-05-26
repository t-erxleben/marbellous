#pragma once

#include <cstring>
#include <vector>

#include "WGLRenderer.hpp"
#include "Scene.hpp"

class WGLSceneRenderer: private WGLRenderer
{
    private:
        void constructBuffers(void** indices, void** vertices, Scene const & scene, size_t & indices_size, size_t & vertices_size);

        std::string const vertex_source{
            R"==(#version 300 es
                in vec2 position;
                in uint colorCode;
                flat out uint colID;
                void main(){
                    gl_Position = vec4(position, 0.0, 1.0);
                    colID = colorCode;
                }
            )=="};
    
        // todo
        std::string const fragment_source{
            R"==(#version 300 es
                precision mediump float;
                flat in uint colID;
                uniform vec3 c0;
                uniform vec3 c1;
                uniform vec3 c2;
                uniform vec3 c3;
                out vec4 fFragment;
                void colorMap(in uint cc, out vec4 color) {
                    switch(cc) {
                        case 0u: color = vec4(c0, 1.0f); break;
                        case 1u: color = vec4(c1, 1.0f); break;
                        case 2u: color = vec4(c2, 1.0f); break;
                        case 3u: color = vec4(c3, 1.0f); break;
                        default: color = vec4(1.0f, 0.0f, 0.5f, 1.0f);
                    }
                }
                void main() {
                    vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
                    colorMap(colID, color);
                    fFragment = color;
                }
            )=="};

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