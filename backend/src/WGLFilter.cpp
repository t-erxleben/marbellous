#include "WGLFilter.hpp"


WGLFilter::WGLFilter(size_t size): curr_size{size}
{
    // build shader
    setupShaderProgram(vertex_source, draw_post_proc_fragment_source, postShader);

    // set up the one triangle to draw
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    GLfloat singeTriangle[6] = {-1.0, -1.0, 3.0, -1.0, -1.0, 3.0};
    glBufferData(GL_ARRAY_BUFFER, 6*sizeof(GLfloat), singeTriangle, GL_STATIC_DRAW);

    // get all uniform locations
    dimLoc = glGetUniformLocation(postShader, "dim");

    // init fbo and textures
    constructFBO(size, false, fbo_post, tex_post);

    // clean up
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void WGLFilter::setActive() const
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glDisable(GL_STENCIL_TEST);
    glDisable(GL_DEPTH_TEST);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glViewport(0, 0, curr_size, curr_size);
    
    glUseProgram(postShader);
}

void WGLFilter::resize()
{
    auto context = WGLContext::getContext();
    size_t res = 0;

    if(Options::getInstance()->getState())
    {
        res = context->getDropRes();
    }
    else
    {
        res = context->getRakeRes();
    }

    resize(res);
}

void WGLFilter::resize(size_t size)
{
    if(curr_size != size)
    {
        curr_size = size;
        glDeleteFramebuffers(1, &fbo_post);
        glDeleteTextures(1, &tex_post);

        constructFBO(size, false, fbo_post, tex_post);
    }
}

void WGLFilter::draw(GLuint src_tex, GLuint target_fbo)
{
    setActive();

    // convolution in first dim
    glBindTexture(GL_TEXTURE_2D, src_tex);
    glBindBuffer(GL_FRAMEBUFFER, fbo_post);
    glUniform1i(dimLoc, 0);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // convolution in second dim
    glBindTexture(GL_TEXTURE_2D, tex_post);
    glBindBuffer(GL_FRAMEBUFFER, target_fbo);
    glUniform1i(dimLoc, 1);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // unbind    
    glBindBuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}