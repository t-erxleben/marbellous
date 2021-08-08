#include "WGLRakeRenderer.hpp"

WGLRakeRenderer::WGLRakeRenderer(WGLSceneRenderer& sr, Scene const & s): curr_tex{0}
{
    auto rakeRes = WGLContext::getContext()->getRakeRes();

    // build shader
    setupShaderProgram(vertex_source, rake_fragment_source, rakeShader);
    setupShaderProgram(vertex_source, draw_fragment_source, drawShader);
    setupShaderProgram(vertex_source, draw_post_proc_fragment_source, postShader);

    // set up the one triangle to draw
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    GLfloat singeTriangle[6] = {-1.0, -1.0, 3.0, -1.0, -1.0, 3.0};
    glBufferData(GL_ARRAY_BUFFER, 6*sizeof(GLfloat), singeTriangle, GL_STATIC_DRAW);

    // get all uniform locations
    colorLoc = glGetUniformLocation(drawShader, "c");
	numColorsLoc = glGetUniformLocation(drawShader, "num_colors");
    nailsLoc = glGetUniformLocation(rakeShader, "nails");
    viscosityLoc = glGetUniformLocation(rakeShader, "viscosity");
    strokeLoc = glGetUniformLocation(rakeShader, "stroke");
    scalingLoc = glGetUniformLocation(rakeShader, "scaling");
    amplitudeLoc = glGetUniformLocation(rakeShader, "amplitude");
    periodLoc = glGetUniformLocation(rakeShader, "period");
    phaseLoc = glGetUniformLocation(rakeShader, "phase");
    dimLoc = glGetUniformLocation(postShader, "dim");
    canvasSizeLoc = glGetUniformLocation(postShader, "canvasSize");

    // init fbo and textures
    constructFBO(rakeRes, false, fbo[0], tex[0]);
    constructFBO(rakeRes, false, fbo[1], tex[1]);
    constructFBO(rakeRes, false, fbo_post[0], tex_post[0]);
    constructFBO(rakeRes, false, fbo_post[1], tex_post[1]);
    constructFBO(rakeRes, false, fbo_screenshot, tex_screenshot);

    reset(sr, s);

    // clean up
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void WGLRakeRenderer::reset(WGLSceneRenderer& sr, Scene const & s)
{
    auto rakeRes = WGLContext::getContext()->getRakeRes();

    char* data = new char[rakeRes*rakeRes*4];
    int len = rakeRes*rakeRes*4;
    sr.drawToBuffer(s, data, len, false);    

    curr_tex = 0;
    glBindTexture(GL_TEXTURE_2D, tex[curr_tex]);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, rakeRes, rakeRes, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glBindTexture(GL_TEXTURE_2D, 0);
    delete[] data;
}

void WGLRakeRenderer::rake(float x, float y, float period, float amplitude, float phase,  GLuint nails[1000])
{
    glUseProgram(rakeShader);
    setActive();
    glBindFramebuffer(GL_FRAMEBUFFER, fbo[!curr_tex]);

    // set uniforms
    glUniform1uiv(nailsLoc, 1000, nails);
    glUniform1f(viscosityLoc, 0.9);
    glUniform1f(scalingLoc, 250.0);
    glUniform2f(strokeLoc, x, y);
    glUniform1f(periodLoc, period);
    glUniform1f(amplitudeLoc, amplitude);
    glUniform1f(phaseLoc, phase);

    // draw call
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // swap textures and fbos
    curr_tex = !curr_tex;

    // unbind
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void WGLRakeRenderer::draw(GLuint target_fbo)
{
    glUseProgram(drawShader);
    setActive();
    
    auto opt = Options::getInstance();
    std::vector<GLfloat> v;
    auto p = *opt->getActivePalette();
    buildColorBuffer(p, v);
    glUniform3fv(colorLoc, p.getSize(), v.data());
	glUniform1i(numColorsLoc, static_cast<int>(p.getSize()));

    // draw to hidden buffer
    //glBindFramebuffer(GL_FRAMEBUFFER, fbo_post[0]);

    glBindFramebuffer(GL_FRAMEBUFFER, target_fbo);

    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    /*
    // draw to canvas
    // this is where post processing might happen
    auto size = WGLContext::getContext()->getRakeRes();
    glUseProgram(postShader);

    // perform conv along x axis
    glUniform1i(dimLoc, 0);
    glUniform1ui(canvasSizeLoc, size);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_post[1]);
    glBindTexture(GL_TEXTURE_2D, tex_post[0]);

    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // perform conv along y axis
    glUniform1i(dimLoc, 1);
    glUniform1ui(canvasSizeLoc, size);
    glBindFramebuffer(GL_FRAMEBUFFER, target_fbo);
    glBindTexture(GL_TEXTURE_2D, tex_post[1]);

    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    */

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void WGLRakeRenderer::drawToBuffer(void* buf, size_t length)
{
    auto rakeRes = WGLContext::getContext()->getRakeRes();

    assert(length == rakeRes * rakeRes * 4);
    draw(fbo_screenshot);
    glReadPixels(0, 0, rakeRes, rakeRes, GL_RGBA, GL_UNSIGNED_BYTE, buf);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void WGLRakeRenderer::setActive() const
{
    auto rakeRes = WGLContext::getContext()->getRakeRes();

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glDisable(GL_STENCIL_TEST);
    glDisable(GL_DEPTH_TEST);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindTexture(GL_TEXTURE_2D, tex[curr_tex]);

    glViewport(0, 0, rakeRes, rakeRes);
}

void WGLRakeRenderer::resize()
{
    glDeleteFramebuffers(2, fbo);
    glDeleteFramebuffers(2, fbo_post);
    glDeleteFramebuffers(1, &fbo_screenshot);
    glDeleteTextures(2, tex);
    glDeleteTextures(2, tex_post);
    glDeleteTextures(1, &tex_screenshot);

    size_t rakeRes = WGLContext::getContext()->getRakeRes();

    constructFBO(rakeRes, false, fbo[0], tex[0]);
    constructFBO(rakeRes, false, fbo[1], tex[1]);
    constructFBO(rakeRes, false, fbo_post[0], tex_post[0]);
    constructFBO(rakeRes, false, fbo_post[1], tex_post[1]);
    constructFBO(rakeRes, false, fbo_screenshot, tex_screenshot);
}
