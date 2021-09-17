#pragma once

#include "WGLRenderer.hpp"
#include "WGLContext.hpp"

#include <cassert>

/**
 * \brief Implements a 3x3 blur filter.
 * 
 **/
class WGLFilter: private WGLRenderer
{
    private:

        ///< Vertex shader code.
        std::string const vertex_source{
            R"==(#version 300 es
                layout (location = 0) in vec2 position;
                out vec2 texCoord;
                void main(){
                    gl_Position = vec4(position, 0., 1.);
                    texCoord = (position + vec2(1.,1.))/2.;
                }
            )=="};

        ///< Fragment shader code implementing a seperable convolution.
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

        ///< Handle to shader program.
        GLint postShader;

        ///< Hidden buffer for first 1D convolution result.
        GLuint fbo_post;
        ///< Color buffer of fbo_post
        GLuint tex_post;
        
        ///< vertex buffer holding one triangle
        GLuint vbo;

        ///< dimension uniform handle
        GLint dimLoc;

        ///< current buffer size
        size_t curr_size;

    public:
        /**
         * \brief Construct a new WGLFilter object.
         * 
         * \param size Buffer size in pixels.
         **/
        WGLFilter(size_t size);

        /**
         * \brief Set the WGL state needed for this shader.
         **/
        void setActive() const override;

        /**
         * \brief Resize all allocated buffers to \p size.
         * 
         * \param size New buffer size.
         **/
        void resize(size_t size);

        /**
         * \brief Resize to the size of the current state (e.g. drop or rake)
         * 
         **/
        void resize() override;

        /**
         * \brief Apply the convolution.
         * 
         * \param src_tex Texture buffer to draw from.
         * \param target_fbo Framebuffer to draw to.
         **/
        void draw(GLuint src_tex, GLuint target_fbo);
};
