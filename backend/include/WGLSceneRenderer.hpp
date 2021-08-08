#pragma once

#include <cstring>
#include <vector>

#include "WGLRenderer.hpp"
#include "Scene.hpp"
#include <cassert>

class WGLSceneRenderer: private WGLRenderer
{
    private:
		size_t i_size, v_size;

        void constructBuffers(GLuint** indices, WGLVertex** vertices, Scene const & scene, size_t & indices_size, size_t & vertices_size);

        std::string const vertex_source{
            R"==(#version 300 es
                in vec2 position;
                in uint colorCode;
				in uint z;
				uniform float disR2;
				uniform vec2 disP;
                flat out uint colID;
                void main(){
					vec2 d = position.xy - disP;
					float s = sqrt(1. + disR2/dot(d,d));
					d = disP + d*s;
					// depth buffer has a resolution from >=16bit
                    gl_Position = vec4(d, float(z) / 65530.f, 1.0);
                    colID = colorCode;
                }
            )=="};
    
        std::string const fragment_source{
            R"==(#version 300 es
                precision mediump float;
                flat in uint colID;
                uniform bool drawColor;
                uniform vec3 c[10];
                out vec4 fFragment;
                
                void main() {
                    vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
                    if(drawColor)
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

        GLint shaderProgram;

        GLuint vbo;
        GLuint ebo;

        GLint colorLoc;
        GLint drawColorLoc;
		GLint locDisR2;
		GLint locDisP;

		GLuint frameBuffer;
		GLuint frameTexture;
		GLuint depthStencilTexture;

		unsigned generation = ~0;
        
    public:
        WGLSceneRenderer();
		void drawToBuffer(const Scene& scene, char* data, int len, bool drawColor = true);
        void drawScene(Scene const & scene, bool drawColor = true);
        void setActive() const override;
        void resize() override;
};
