#pragma once

#include "WGLRenderer.hpp"

#include "WGLSceneRenderer.hpp"

class WGLRakeRenderer: private WGLRenderer
{
    private:

        // both shaders will be using that
        std::string const vertex_source{
            R"==(#version 300 es
                in vec2 position;
                out vec2 texCoord;
                void main(){
                    gl_Position = vec4(position, 0.0, 1.0);
                    texCoord = vec2((position.x+1.0)/2.0, (position.y+1.0)/2.0);
                }
            )=="};
    
        std::string const rake_fragment_source{
            R"==(#version 300 es
                precision mediump float;
                uniform bool nails[];
                uniform vec2 stroke;
                uniform float viscosity;
                uniform float scaling;
                uniform sampler2D tex;
                in vec2 texCoord;
                out vec4 fFragment;
 
                void main() {
                    // todo calc distance d
                    vec2 orig = texCoord - stroke * pow(viscosity, scaling * d);
					fFragment = texture(tex, orig);
                }
            )=="};

        std::string const draw_fragment_source{
            R"==(#version 300 es
                precision mediump float;
                uniform vec3 c0;
                uniform vec3 c1;
                uniform vec3 c2;
                uniform vec3 c3;
                uniform sampler2D tex;
                in vec2 texCoord;
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
                    colorMap(texture(tex, texCoord), color);
                    fFragment = color;
                }
            )=="};

        // should be used to draw the texture to the canvas
        // substitutes color codes for actual colors
        // expects to be used to draw exactly one triangle defined by (-1, -1), (-1, 3), (3, -1)
        GLint drawShader;

        void loadTexFromScene(WGLSceneRenderer const * sr);

    public:

        WGLRakeRenderer(WGLSceneRenderer const * sr);
        void reset(WGLSceneRenderer const * sr);
        void draw();
        void drawToBuffer(char *data, int len);



};