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
    
        // todo
        std::string const fragment_source{
            R"==(#version 300 es
                precision mediump float;
                flat in uint colID;
                uniform vec3 c0;
                uniform vec3 c1;
                uniform vec3 c2;
                uniform vec3 c3;
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
                    colorMap(colID, color);
					fFragment = color;
                }
            )=="};

        GLint shaderProgram;

        GLuint vao;
        GLuint ebo;

        GLint color0Loc;
        GLint color1Loc;
        GLint color2Loc;
        GLint color3Loc;
		GLint locDisR2;
		GLint locDisP;

		GLuint frameBuffer;
		GLuint frameTexture;
		GLuint depthStencilTexture;

		unsigned generation = ~0;
        
    public:
        WGLSceneRenderer();
		void drawToBuffer(const Scene& scene, char* data, int len);
        void drawScene(Scene const & scene);
        void setActive() const;
};
