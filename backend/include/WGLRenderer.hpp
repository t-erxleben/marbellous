#pragma once

#include <string>
#include <GLES3/gl3.h>
#include <EGL/egl.h>

#include "Palette.hpp"

class WGLRenderer
{
public:
    GLuint compileShader(GLenum const type, std::string const shader) const;
    void setupShaderProgram(std::string vert, std::string frag, GLint& shader);
	virtual void setActive() const = 0;
    
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
