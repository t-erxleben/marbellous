#pragma once

#include <cstring>
#include <vector>

#include "WGLRenderer.hpp"
#include "Scene.hpp"

class WGLSceneRenderer: private WGLRenderer
{
    private:
		size_t i_size, v_size;

        void constructBuffers(GLuint** indices, WGLVertex** vertices, Scene const & scene, size_t & indices_size, size_t & vertices_size);

		std::string vertex_canvas_source = 
			R"==(#version 300 es
			in vec2 position;
			in uint colorCode;
			in uint z;
			out vec2 tex;
			void main() {
				switch (z) {
					case uint(0): tex = vec2(0.25, 1.25); break;
					case uint(1): tex = vec2(.25, .25); break;
					case uint(2): tex = vec2(1.25, 0.25); break;
				}
				gl_Position = vec4(position, 0, 1.0);
			}
		)==";

		std::string fragment_canvas_source = 
			R"==(#version 300 es
			precision mediump float;
			uniform sampler2D samp;
			in vec2 tex;
			out vec4 fColor;
			void main() {
				fColor = texture(samp, tex)+
						 texture(samp, tex + vec2(0.5,0.5))+
						 texture(samp, tex + vec2(0,0.5))+
						 texture(samp, tex + vec2(0.5,0));
			}

		)==";


        std::string vertex_source = 
            R"==(#version 300 es
                in vec2 position;
                in uint colorCode;
				in uint z;
				uniform float disR2;
				uniform vec2 disP;
                flat out uint colID;
                void main(){
					vec2 d = position.xy - disP;
					if(abs(d.x) > 1.) { d.x = -sign(d.x)*(2. - abs(d.x)); }
					if(abs(d.y) > 1.) { d.y = -sign(d.y)*(2. - abs(d.y)); }
					float s = sqrt(1. + disR2/dot(d,d));
                    gl_Position = vec4((position.xy + d*(s-1.))*0.5, float(z) / 4294967295.f, 1.0);
                    colID = colorCode;
                }
            )==";
    
        std::string fragment_source = 
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
            )==";

        GLuint vao;
        GLuint ebo;
		GLuint vaoTriangle;
		GLuint shaderProgram;

        GLint color0Loc;
        GLint color1Loc;
        GLint color2Loc;
        GLint color3Loc;
		GLint locDisR2;
		GLint locDisP;

		GLuint frameBuffer;
		GLuint frameTexture;

		struct {
			GLuint frameBuffer;
			GLuint frameTexture;
			GLuint program;
		} wrapping;

		unsigned generation = ~0;
        
    public:
        WGLSceneRenderer();
		void drawToBuffer(const Scene& scene, char* data, int len);
        void drawScene(Scene const & scene);
        void setActive();
};
