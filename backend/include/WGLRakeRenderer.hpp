#pragma once

#include "WGLRenderer.hpp"
#include "WGLFilter.hpp"
#include "WGLSceneRenderer.hpp"

#include <cassert>

class WGLRakeRenderer: private WGLRenderer
{
    private:

        WGLFilter *conv;

        // both shaders will be using that
        std::string const vertex_source{
            R"==(#version 300 es
                layout (location = 0) in vec2 position;
                out vec2 texCoord;
                void main(){
                    gl_Position = vec4(position, 0., 1.);
                    texCoord = (position + vec2(1.,1.))/2.;
                }
            )=="};
    
        std::string const rake_fragment_source{
            R"==(#version 300 es
                precision mediump float;
                uniform bool nails[1000];
                uniform vec2 stroke;
                uniform float viscosity;
                uniform float scaling;
                uniform float period;
                uniform float amplitude;
                uniform float phase;
                uniform sampler2D tex;
                in vec2 texCoord;
                out vec4 fFragment;

				// distance is an opengl function and should not be overwitten
                float dis(float x, float y)
                {
                    if(abs(x-y) > 0.5)
                        return 1. - abs(x-y);
                    return abs(x-y);
                }

                // scaled to period length and amplitude
                float scaled_sin(float x)
                {
                    const float pi = 3.1415926535897932384626433832795;
                    return sin((x - phase) * 2. * pi / period) * amplitude;
                }
 
                void main() {
                    const float eps = 0.000001;
                    int i, dim;
                    float nailPos, d;
                    vec2 shift = vec2(0.0, 0.0);

                    // assuming only strokes in x and y direction
                    dim = (abs(dot(stroke, vec2(1.0, 0.0))) < eps) ? 0 : 1;
					float up = dim == 0 ? 1. : -1.;

                    // for each nail
                    for(i = 0; i < 1000; ++i)
                    {
                        nailPos = float(i) / float(nails.length());
                        // wavines corrected distance to rake nail
                        d = dis(
                            texCoord[dim] - scaled_sin(texCoord[1-dim]) * up,
                            nailPos
                        );
                        shift += (stroke * pow(viscosity, scaling * d)) * float(nails[i]);
                    }

                    // linear origin position
                    vec2 orig = texCoord - shift;
                    
                    // correct for waviness of lookup point
                    orig[dim] = orig[dim] + (scaled_sin(orig[1-dim]) - scaled_sin(texCoord[1-dim]))*up;

					fFragment = texture(tex, orig);
                }
            )=="};

        std::string const draw_fragment_source{
            R"==(#version 300 es
                precision mediump float;
                uniform vec3 c[10];
				uniform int num_colors;
                uniform sampler2D tex;
                in vec2 texCoord;
                out vec4 fFragment;

                void main() {
                    vec4 color = vec4(0.0, 0.0, 0.0, 1.0);
                    int colID = int(texture(tex, texCoord).r*256.);
                    if(colID == 0) {
                        discard;
                    }
					else if(colID <= num_colors)
                    {
                        color = vec4(c[colID-1], 1.0);
                    }
                    else
                    {
                        // bright magenta for default color
                        color = vec4(1.0, 0.0, 1.0, 1.0);
                    }
                    fFragment = color;
                }
            )=="};

        // should be used to draw the texture to the canvas or another buffer
        // substitutes color codes for actual colors
        // expects to be used to draw exactly one triangle defined by (-1, -1), (-1, 3), (3, -1)
        GLint drawShader;

        GLint rakeShader;

        // Frame buffer with to textures; one to load from and one to draw to
        GLuint fbo[2], fbo_screenshot;
        GLuint tex_screenshot;
        GLuint tex[2];
        uint8_t curr_tex;
        
        // one and single triangle
        GLuint vbo;

        // uniform locations
        GLint colorLoc;
        GLint numColorsLoc;
        GLint nailsLoc;
        GLint strokeLoc;
        GLint viscosityLoc;
        GLint scalingLoc;
        GLint amplitudeLoc;
        GLint periodLoc;
        GLint phaseLoc;

    public:
        WGLRakeRenderer(WGLSceneRenderer& sr, Scene const & s);
        void setActive() const override;
        void reset(WGLSceneRenderer& sr, Scene const & s);
        void resize() override;
        void rake(float x, float y, float period, float amplitude, float phase,  GLint nails[1000]);
        void draw(GLuint target_fbo = 0);
        void drawToBuffer(void* buf, size_t length);
        void swapBuffers();
};
