#pragma once

#include <string>
#include <GLES3/gl3.h>

#include "Palette.hpp"

class WGLRenderer
{
private:
    GLuint compileShader(GLenum const type, std::string const shader) const;
public:
    void setupShaderProgram(std::string vert, std::string frag, GLint& shader);
	void constructFBO(size_t x, bool stencil_depth, GLint interpolation, GLuint& fbo, GLuint& tex);
	virtual void setActive() const = 0;
	virtual void resize() = 0;
    
    void buildColorBuffer(Palette const & p, std::vector<GLfloat>& v)
    {
	    for(Color const & c: p)
	    {
		    v.push_back(static_cast<GLfloat>(std::get<0>(c.getRGB()))/255.0);
		    v.push_back(static_cast<GLfloat>(std::get<1>(c.getRGB()))/255.0);
		    v.push_back(static_cast<GLfloat>(std::get<2>(c.getRGB()))/255.0);
	    }
    }
};
