#pragma once

#include "WGLRenderer.hpp"

#include <cassert>

class WGLFilter: private WGLRenderer
{
    private:

        std::string const vertex_source{
            R"==(#version 300 es
                layout (location = 0) in vec2 position;
                out vec2 texCoord;
                void main(){
                    gl_Position = vec4(position, 0., 1.);
                    texCoord = (position + vec2(1.,1.))/2.;
                }
            )=="};

        std::string const draw_post_proc_fragment_source{
            R"==(#version 300 es
                precision mediump float;
                uniform sampler2D tex;
                uniform int dim;
                in vec2 texCoord;
                out vec4 fFragment;

                void main() 
                {
                    // calculate a 1D Convolution along dim
                    float stepSize = 1./float(textureSize(tex, 0)[dim]);
                    vec2 left = vec2(0.,0.);
                    vec2 right = vec2(0.,0.);
                    left[dim] = -stepSize;
                    right[dim] = stepSize;       

                    // binom filter
                    fFragment = 0.25 * texture(tex, texCoord + left) + 0.5*texture(tex, texCoord) + 0.25 * texture(tex, texCoord + right);
                }

            )=="};

        // same loke drawShader but expects to read real colors instead of color codes
        // used for post processing
        GLint postShader;

        // Frame buffer with to textures; one to load from and one to draw to
        GLuint fbo_post[2];
        GLuint tex_post[2];
        
        // one and single triangle
        GLuint vbo;

        GLint dimLoc;

    public:
        WGLFilter();
        void setActive() const override;
        void resize() override;
        void draw(GLuint target_fbo = 0);
};

