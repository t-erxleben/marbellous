#pragma once

#include <cstring>
#include <vector>

#include "WGLRenderer.hpp"
#include "WGLFilter.hpp"
#include "Scene.hpp"
#include <cassert>

/**
 * \brief Render scenes to buffers.
 * 
 * \todo explain draw algorithm
 * 
 **/
class WGLSceneRenderer: private WGLRenderer
{
    private:
		size_t i_size, v_size;
        WGLFilter *conv;

        void constructBuffers(GLuint** indices, WGLVertex** vertices, Scene const & scene, size_t & indices_size, size_t & vertices_size);

        ///< Vertex shader code. Implements displacement for up to 100 new circle objects.
        std::string const vertex_source{
            R"==(#version 300 es
                in vec2 position;
                in uint colorCode;
				in uint z;
				uniform vec3[100] dis;
				uniform int count;
                flat out uint colID;
                void main(){
					vec2 pos = position.xy;
					if( count > 0 ) {
						pos = vec2(0, 0);
						for(int i = 0; i < count; ++i) {
							vec2 d = position.xy - dis[i].xy;
							float s = sqrt(1. + dis[i].z*dis[i].z/dot(d,d));
							pos += dis[i].xy + d*s - position.xy;
						}
						pos += position.xy;
					}
					// depth buffer has a resolution from >=16bit
                    gl_Position = vec4(pos , float(z) / 65530.f, 1.0);
                    colID = colorCode;
                }
            )=="};
    
        ///< Fragment shader code. Can draw either color or color codes.
        std::string const fragment_source{
            R"==(#version 300 es
                precision mediump float;
                flat in uint colID;
                uniform int drawColor;
                uniform vec3 c[10];
                out vec4 fFragment;
                
                void main() {
                    vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
                    if(drawColor != 0)
                    {
                        if(int(colID) < c.length())
                        {
                            color = vec4(c[colID], 1.0);
                        }
                        else
                        {
                            // bright magenta for default color
                            color = vec4(1.0, 0.0, 1.0, 1.0);
                        }
                    }
                    else
                    {
                        // draw color codes in red channel, intended to be used to generate code maps for raking
                        color = vec4(float(colID + uint(1)) / 256., 0.0, 0.0, 0.0);
                    }
					fFragment = color;
                }
            )=="};

        ///< Shader program handle
        GLint shaderProgram;

        ///< vertex buffer object holding all vertices
        GLuint vbo;
        ///< element buffer holding all indices for drawing orgder of vertices.
        GLuint ebo;

        ///< uniform position handles
        GLint colorLoc;
        GLint drawColorLoc;
		GLint locDis;
		GLint locDisCount;

        ///< FBO used for screenshots
		GLuint frameBuffer;
		GLuint frameTexture;
		GLuint depthStencilTexture;

        ///< Generation of drawing. See Scene for explenation.
		unsigned generation = ~0;
        
    public:

        /**
         * \brief Construct a new WGLSceneRenderer object
         * 
         **/
        WGLSceneRenderer();

        /**
         * \brief Draw Scene to a given buffer.
         * 
         * Used for screenshots.
         * 
         * \param scene Scene to draw.
         * \param data Prealoocated buffer to use.
         * \param len Length of \p data.
         * \param drawColor Wether to draw in color or color codes. Defaults to color.
         **/
		void drawToBuffer(const Scene& scene, char* data, int len, bool drawColor = true);

        /**
         * \brief Draw the scene.
         * 
         * \param scene Scene object to draw.
         * \param drawColor Wether to draw in color or color codes. Defaults to color.
         **/
        void drawScene(Scene const & scene, bool drawColor = true);

        /**
         * \brief Set the WebGL state needed for the shader.
         * 
         **/
        void setActive() const override;

        /**
         * \brief Resize to the resolution set for rake state in WGLContext 
         * 
         **/
        void resize() override;
};
