#include "WGLRakeRenderer.hpp"

WGLRakeRenderer::WGLRakeRenderer(WGLSceneRenderer& sr, Scene const & s)
{
    // set up the one triangle to draw
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    GLfloat singeTriangle[6] = {-1.0, -1.0, 3.0, -1.0, -1.0, 3.0};
    glBufferData(GL_ARRAY_BUFFER, 6, singeTriangle, GL_STATIC_DRAW);

    // build shader
    setupShaderProgram(vertex_source, rake_fragment_source, rakeShader);
    setupShaderProgram(vertex_source, draw_fragment_source, drawShader);

    // get all uniform locations
    color0Loc = glGetUniformLocation(drawShader, "c0");
    color1Loc = glGetUniformLocation(drawShader, "c1");
    color2Loc = glGetUniformLocation(drawShader, "c2");
    color3Loc = glGetUniformLocation(drawShader, "c3");
    nailsLoc = glGetUniformLocation(rakeShader, "nails");
    viscosityLoc = glGetUniformLocation(rakeShader, "viscosity");
    strokeLoc = glGetUniformLocation(rakeShader, "stroke");
    scalingLoc = glGetUniformLocation(rakeShader, "scaling");

    //set attrib (should just work for both shader because layout is set in shader code)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // get rendered scene 
    // todo: this needs to be in color codes; see issue #94
    std::vector<char> data(720*720*4);
    int len = data.size();
    sr.drawToBuffer(s, data.data(), len);

    // init fbo
    glGenFramebuffers(1, &fbo);
    
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    
    // init textures
    glGenTextures(2, tex);
    curr_tex = 1;
    glBindTexture(GL_TEXTURE_2D, tex[curr_tex]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 720, 720, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex[curr_tex], 0);

    curr_tex = 0;
    glBindTexture(GL_TEXTURE_2D, tex[curr_tex]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 720, 720, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // set drawing buffer
    GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, drawBuffers);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        fprintf(stderr, "failed to create framebuffer for raking!");
    }

    // init screenshot fbo
    glGenFramebuffers(1, &fbo_screenshot);
    GLuint colBuff;
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_screenshot);
    glGenTextures(1, &colBuff);
    glBindTexture(GL_TEXTURE_2D, colBuff);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 720, 720, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_screenshot, 0);
    GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, drawBuffers);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        fprintf(stderr, "failed to create framebuffer for rake screenshot!");
    }

    // unbind
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void WGLRakeRenderer::reset(WGLSceneRenderer& sr, Scene const & s)
{
    // get rendered scene
    std::vector<char> data(720*720*4);
    int len = data.size();
    sr.drawToBuffer(s, data.data(), len);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    
    curr_tex = 1;
    glBindTexture(GL_TEXTURE_2D, tex[curr_tex]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 720, 720, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex[curr_tex], 0);

    curr_tex = 0;
    glBindTexture(GL_TEXTURE_2D, tex[curr_tex]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 720, 720, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void WGLRakeRenderer::rakeAndDraw(float x, float y, float speed, bool nails[1000])
{
    glUseProgram(rakeShader);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glBindTexture(GL_TEXTURE0, tex[curr_tex]);

    // copy for now
    GLuint* nailsi = new GLuint[1000];
    for(int i = 0; i < 1000; ++i) nailsi[i] = nails[i];

    // set uniforms
    glUniform1uiv(nailsLoc, 1000, nailsi);
    glUniform1f(viscosityLoc, 0.9);
    glUniform1f(scalingLoc, 1.0);
    glUniform2f(strokeLoc, x*speed, y*speed);

    // todo draw call
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // swap textures
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex[curr_tex], 0);
    curr_tex = !curr_tex;
    glBindTexture(GL_TEXTURE0, tex[curr_tex]);

    // change shader
    glUseProgram(drawShader);

    // draw to screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE0, 0);
}

void WGLRakeRenderer::drawToBuffer(void* buf, size_t& length)
{
    assert(length == 720 * 720 * 4);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_screenshot);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glUseProgram(drawShader);
    
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glReadPixels(0, 0, 720, 720, GL_RGBA, GL_UNSIGNED_BYTE, buf);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}