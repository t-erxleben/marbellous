#pragma once

#include <string>
#include <GLES3/gl3.h>

#include "Palette.hpp"

/**
 * \brief Base class for rendering.
 * 
 * Provides some basic function like shader compilation, FBO creation and color packing.
 * 
 **/
class WGLRenderer
{
private:

	/**
	 * \brief Compile a shader
	 * 
	 * \param type Type of the shader.
	 * \param shader Shader code.
	 * \return Shader handle.
	 **/
    GLuint compileShader(GLenum const type, std::string const shader) const;

public:
	/**
	 * \brief Create a shader program.
	 * 
	 * This includes error checking and linking of a vertex shader and a fragment shader.
	 * 
	 * \param vert Vertex shader code.
	 * \param frag Fragment shader code.
	 * \param shader Shader program handle.
	 **/
    void setupShaderProgram(std::string vert, std::string frag, GLint& shader);

	/**
	 * \brief Construct a FBO.
	 * 
	 * \param x Pixel resolution for the FBO.
	 * \param stencil_depth Wether to generate a stencil/depth buffer.
	 * \param interpolation used on texture sampling
	 * \param[out] fbo FBO handle.
	 * \param[out] tex Handle for the FBO draw attachement which can be used as texture.
	 **/
	void constructFBO(size_t x, bool stencil_depth, GLint interpolation, GLuint& fbo, GLuint& tex);

	/**
	 * \brief All Renderer objects need to be set active to set the needed WGL state.
	 * 
	 **/
	virtual void setActive() const = 0;

	/**
	 * \brief All Renderer objects should be resizeable.
	 * 
	 **/
	virtual void resize() = 0;
    
	/**
	 * \brief Build a color buffer from a palette which may be send to GPU.
	 * 
	 * \param p A palette.
	 * \param v The color buffer to fill.
	 **/
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
