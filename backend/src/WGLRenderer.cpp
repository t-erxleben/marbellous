#include "WGLRenderer.hpp"


GLuint WGLRenderer::compileShader(GLenum type, std::string source) const{

    GLuint shader = glCreateShader(type);

    char const * legacy = source.c_str();

    if (shader == 0) {
        printf("Could not create shader!");
        return 0;
    }

    // Assign and compile the source to the shader object

    int size = (int) source.size();
    glShaderSource(shader, 1, &legacy, &size);
    glCompileShader(shader);

    // Check if there were errors
    int infoLen = 0;
    glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &infoLen );
    if (infoLen > 1) {
        char infoLog[infoLen];
        // And print them out
        glGetShaderInfoLog( shader, infoLen, NULL, infoLog );
        printf("%s", infoLog);
    }


    return shader;
}

void WGLRenderer::setupShaderProgram(std::string vert, std::string frag, GLint& shader)
{
    GLint fragmentShader = compileShader(GL_FRAGMENT_SHADER, frag);
    GLint vertexShader = compileShader(GL_VERTEX_SHADER, vert);

    shader = glCreateProgram();
    glAttachShader(shader, vertexShader);
    glAttachShader(shader, fragmentShader);

    glLinkProgram(shader);
    glValidateProgram(shader);
}

void WGLRenderer::constructFBO(size_t x, bool stencil_depth, GLuint& fbo, GLuint& tex)
{
    GLuint depthStencilTexture;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, x, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);        
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
	
    if(stencil_depth)
    {
        glGenTextures(1, &depthStencilTexture);
        glBindTexture(GL_TEXTURE_2D, depthStencilTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, x, x, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthStencilTexture, 0);
        GLenum drawBuffers[2] = {GL_COLOR_ATTACHMENT0, GL_DEPTH_STENCIL_ATTACHMENT};
        glDrawBuffers(1, drawBuffers);
    }
    else
    {
        GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
        glDrawBuffers(1, drawBuffers);
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        fprintf(stderr, "failed to create framebuffer!");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
        fbo = 0;
        tex = 0;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}