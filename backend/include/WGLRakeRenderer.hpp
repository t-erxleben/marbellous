#pragma once

#include "WGLRenderer.hpp"

#include "WGLSceneRenderer.hpp"

#include <cassert>

class WGLRakeRenderer: private WGLRenderer
{
    private:

        // both shaders will be using that
        std::string const vertex_source{
            R"==(#version 300 es
                layout (location = 0) in vec2 position;
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
                    const float eps = 0.000001;
                    int i;
                    int dir;
                    float d;
                    float nailPos;
                    vec2 shift = vec2(0.0, 0.0);

                    // assuming only strokes in x and y direction
                    dir = (dot(stroke, vec2(1.0, 0.0) < eps) ? 0 : 1;

                    for(i = 0; i < nails.length(); ++i)
                    {
                        if(nails[i])
                        {
                            // 2.0 is the side length of the canvas
                            nailPos = float(i) * 2.0 / float(nails.length())
                            d = abs(gl_FragCoord[dir] - nailPos)
                            shift += stroke * pow(viscosity, scaling * d);
                        }
                    }
                    vec2 orig = texCoord - shift;
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

        GLint rakeShader;

        // Frame buffer with to textures; one to load from and one to draw to
        GLuint fbo, fbo_screenshot;
        GLuint tex[2];
        uint8_t curr_tex;
        
        // one and single triangle
        GLuint vbo;

        // uniform locations
        GLint color0Loc;
        GLint color1Loc;
        GLint color2Loc;
        GLint color3Loc;
        GLint nailsLoc;
        GLint strokeLoc;
        GLint viscosityLoc;
        GLint scalingLoc;

        void loadTexFromScene(WGLSceneRenderer const & sr);
        void drawFromTexture();

    public:

        WGLRakeRenderer(WGLSceneRenderer& sr, Scene const & s);
        void reset(WGLSceneRenderer& sr, Scene const & s);
        void rakeAndDraw(float x, float y, float speed, bool nails[100]);
        void drawToBuffer(void* buf, size_t& length);
};