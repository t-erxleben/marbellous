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
                //uniform float period;
                //uniform float amplitude;
                uniform sampler2D tex;
                in vec2 texCoord;
                out vec4 fFragment;

                float distance(float x, float y)
                {
                    if(abs(x-y) > 0.5)
                        return 1. - abs(x-y);
                    return abs(x-y);
                }

                // scaled to period length and amplitude
                float scaled_sin(float x)
                {
                    const float pi = 3.1415926535897932384626433832795;
                    float period = 1.;
                    float amplitude = 0.1;
                    return sin(x * 2. * pi * period) * amplitude;
                }
 
                void main() {
                    const float eps = 0.000001;
                    int i, dim;
                    float nailPos, d;
                    vec2 shift = vec2(0.0, 0.0);

                    // assuming only strokes in x and y direction
                    dim = (abs(dot(stroke, vec2(1.0, 0.0))) < eps) ? 0 : 1;

                    // for each nail
                    for(i = 0; i < 1000; ++i)
                    {
                        if(nails[i])
                        {
                            nailPos = float(i) / float(nails.length());
                            d = distance(texCoord[dim] - scaled_sin(texCoord[1-dim]), nailPos);
                            shift += stroke * pow(viscosity, scaling * d);
                        }
                    }

                    // linear origin position
                    vec2 orig = texCoord - shift;
                    
                    // correct for waviness of lookup point
                    orig[dim] = orig[dim] + scaled_sin(orig[1-dim]) - scaled_sin(texCoord[1-dim]);

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

        std::string const draw_post_proc_fragment_source{
            R"==(#version 300 es
                precision mediump float;
                uniform sampler2D tex;
                in vec2 texCoord;
                out vec4 fFragment;

                void main() 
                {
                    fFragment = texture(tex, texCoord);
                }

            )=="};

        // should be used to draw the texture to the canvas or another buffer
        // substitutes color codes for actual colors
        // expects to be used to draw exactly one triangle defined by (-1, -1), (-1, 3), (3, -1)
        GLint drawShader;

        // same loke drawShader but expects to read real colors instead of color codes
        // used for post processing
        GLint postShader;

        GLint rakeShader;

        // Frame buffer with to textures; one to load from and one to draw to
        GLuint fbo, fbo_screenshot;
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

    public:

        WGLRakeRenderer(WGLSceneRenderer& sr, Scene const & s);
        void setActive() const override;
        void reset(WGLSceneRenderer& sr, Scene const & s);
        void rake(float x, float y, GLuint nails[1000]);
        void draw();
        void drawToBuffer(void* buf, size_t length);
};
