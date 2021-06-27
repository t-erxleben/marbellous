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
                uniform bool nails[1000];
                uniform vec2 stroke;
                uniform float viscosity;
                uniform float scaling;
                uniform sampler2D tex;
                in vec2 texCoord;
                out vec4 fFragment;
 
                void main() {
                    const float eps = 0.000001;
                    int i;
                    int dim;
                    float nailPos;
                    float d;
                    vec2 shift = vec2(0.0, 0.0);
                    vec2 str = vec2(0.0, 0.0);

                    // assuming only strokes in x and y direction
                    dim = (dot(stroke, vec2(1.0, 0.0)) < eps) ? 0 : 1;

                    // for each nail
                    for(i = 0; i < nails.length(); ++i)
                    {
                        if(nails[i])
                        {
                            nailPos = float(i) / float(nails.length());
                            d = abs(texCoord[dim] - nailPos);
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
        void rake(float x, float y, float speed, GLuint nails[1000]);
        void draw();
        void drawToBuffer(void* buf, size_t length);
};
