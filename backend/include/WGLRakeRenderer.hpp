#pragma once

#include "WGLRenderer.hpp"
#include "WGLFilter.hpp"
#include "WGLSceneRenderer.hpp"

#include <cassert>

/**
 * \brief Implementation of raking.
 * 
 * For rakes the data and rendering are not decoupled as Scene objects and WGLSceneRenderer objects are.
 * This was a more simple approach which was also very beneficial for performance due to offloading to GPU.
 * 
 **/
class WGLRakeRenderer: private WGLRenderer
{
    private:

        ///< Pointer to a filter which might be applied.
        WGLFilter *conv;

        ///< Vertex shader source used by all shader programs.
        std::string const vertex_source{
            R"==(#version 300 es
                layout (location = 0) in vec2 position;
                out vec2 texCoord;
                void main(){
                    gl_Position = vec4(position, 0., 1.);
                    texCoord = (position + vec2(1.,1.))/2.;
                }
            )=="};
    
        ///< Fragment shader code implementing rakes based on the model of Prof. Graham Horton.
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

        ///< Fragment shader for the shader program used to draw rake results.
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

        /** 
         * \brief Shader for drawing.
         * Should be used to draw the texture to the canvas or another buffer.
         * Substitutes color codes for actual colors.
         * Expected to be used to draw exactly one triangle defined by (-1, -1), (-1, 3), (3, -1).
         **/ 
        GLint drawShader;

        /**
         * \brief Shader for raking.
         * Draws color codes to internal buffers.
         * Expected to be used to draw exactly one triangle defined by (-1, -1), (-1, 3), (3, -1)
         **/
        GLint rakeShader;

        ///< Framebuffers used for raking and taking screenshots.
        GLuint fbo[2], fbo_screenshot;
        GLuint tex_screenshot;
        GLuint tex[2];
        ///< current internal texture to draw from.
        uint8_t curr_tex;
        
        ///< Vertex buffer holding one triangle.
        GLuint vbo;

        ///< uniform location handles
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
        /**
         * \brief Construct a new WGLRakeRenderer object.
         * 
         * \param sr WGLSceneRenderer used to render a color code image from Scene \p s
         * \param s Scene to use as initial state.
         **/
        WGLRakeRenderer(WGLSceneRenderer& sr, Scene const & s);

        /**
         * \brief Set the WebGL state needed for the shader.
         * 
         **/
        void setActive() const override;

        /**
         * \brief Resets the WGLRakeRenderer to a new initial state.
         * 
         * \param sr WGLSceneRenderer used to render a color code image from Scene \p s
         * \param s Scene to use as initial state.
         **/
        void reset(WGLSceneRenderer& sr, Scene const & s);

        /**
         * \brief Resize to the resolution set for rake state in WGLContext 
         * 
         **/
        void resize() override;

        /**
         * \brief Compute a rake step.
         * 
         * For amplitude values greater than 0 this results in a wavy rake movement.
         * \ref curr_tex will be toggled afterwards so that the hidden color code buffers will be switched.
         * The rake stroke strength is encoded by \p x and \p y though other directions than horizontal or vertical are not supported (yet).
         * 
         * \param x x-direction of the stroke
         * \param y y-direction of the stroke
         * \param period Period length of wavy movement.
         * \param amplitude Amplitude of wavy movement.
         * \param phase Phase shift of wavy movement.
         * \param nails Rake representation. 1000 possible equally positioned tines with TRUE if the tine is present and FALSE otherwise.
         **/
        void rake(float x, float y, float period, float amplitude, float phase,  GLint nails[1000]);

        /**
         * \brief Draw the current raked stated to \p target_fbo
         * 
         * The default 0 is the visible context in the frontend.
         * 
         * \param target_fbo Framebuffer to draw to.
         **/
        void draw(GLuint target_fbo = 0);

        /**
         * \brief Save the current rake state to a buffer.
         * 
         * Used for screenshots.
         * 
         * \param buf Preallocated buffer to use.
         * \param length Size of the buffer.
         **/
        void drawToBuffer(void* buf, size_t length);

        /**
         * \brief Swap internal rake state buffers.
         * 
         * This efficiently implements a one step undo.
         * 
         **/
        void swapBuffers();
};
