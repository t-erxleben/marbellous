#include "WGLRakeRenderer.hpp"

WGLRakeRenderer::WGLRakeRenderer(WGLSceneRenderer& sr, Scene const & s)
{
    auto rakeRes = WGLContext::getContext()->getRakeRes();
    char* data = new char[rakeRes*rakeRes*4];
    int len = rakeRes*rakeRes*4;
    sr.drawToBuffer(s, data, len, false);

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

    // init fbo
    glGenFramebuffers(1, &fbo);
    
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    
    // init textures
    glGenTextures(2, tex);

    curr_tex = 1;
    glBindTexture(GL_TEXTURE_2D, tex[curr_tex]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rakeRes, rakeRes, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex[curr_tex], 0);

    curr_tex = 0;
	glBindTexture(GL_TEXTURE_2D, tex[curr_tex]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rakeRes, rakeRes, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // set drawing buffer
    GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, drawBuffers);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        fprintf(stderr, "failed to create framebuffer for raking!\n");
    }

    // init screenshot fbo
    glGenFramebuffers(1, &fbo_screenshot);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_screenshot);
    glGenTextures(1, &tex_screenshot);
    glBindTexture(GL_TEXTURE_2D, tex_screenshot);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rakeRes, rakeRes, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_screenshot, 0);

    glDrawBuffers(1, drawBuffers);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        fprintf(stderr, "failed to create framebuffer for rake screenshot! state is: %i\n", glCheckFramebufferStatus(GL_FRAMEBUFFER));
    }

    // unbind
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    delete[] data;
}

void WGLRakeRenderer::reset(WGLSceneRenderer& sr, Scene const & s)
{
    auto rakeRes = WGLContext::getContext()->getRakeRes();

    char* data = new char[rakeRes*rakeRes*4];
    int len = rakeRes*rakeRes*4;
    sr.drawToBuffer(s, data, len, false);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    
    curr_tex = 1;
    glBindTexture(GL_TEXTURE_2D, tex[curr_tex]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex[curr_tex], 0);

    curr_tex = 0;
    glBindTexture(GL_TEXTURE_2D, tex[curr_tex]);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, rakeRes, rakeRes, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    delete[] data;
}

void WGLRakeRenderer::rake(float x, float y, GLuint nails[1000])
{
    glUseProgram(rakeShader);
    setActive();
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex[!curr_tex], 0);

    // set uniforms
    glUniform1uiv(nailsLoc, 1000, nails);
    glUniform1f(viscosityLoc, 0.9);
    // FIXME set scaling to some actually meaningful value
    glUniform1f(scalingLoc, 200.0);
    glUniform2f(strokeLoc, x, y);

    // draw call
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // swap textures
    curr_tex = !curr_tex;

    // unbind
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void WGLRakeRenderer::draw()
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
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_screenshot);

    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // draw to canvas
    // it would be possible to do more preprocessing, just modify the shader
    // for now we only use linear interpolation
    glUseProgram(postShader);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, tex_screenshot);

    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void WGLRakeRenderer::drawToBuffer(void* buf, size_t length)
{
    auto rakeRes = WGLContext::getContext()->getRakeRes();

    assert(length == rakeRes * rakeRes * 4);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_screenshot);
    draw();
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
