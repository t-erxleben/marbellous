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
