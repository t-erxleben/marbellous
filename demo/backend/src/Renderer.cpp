#include "Renderer.hpp"

Renderer::Renderer(std::string canvasID)
{
    setupContext(canvasID);
    setupShaderProgram();
}

void Renderer::setupContext(std::string canvasID) {
    emscripten_set_canvas_element_size(canvasID.c_str(), 720, 720);
    EmscriptenWebGLContextAttributes attrs;
    emscripten_webgl_init_context_attributes(&attrs);

    attrs.explicitSwapControl = 0;
    attrs.depth = 1;
    attrs.stencil = 1;
    attrs.antialias = 1;
    attrs.majorVersion = 2;
    attrs.minorVersion = 0;

    context = emscripten_webgl_create_context(canvasID.c_str(), &attrs);
    emscripten_webgl_make_context_current(context);
}

void Renderer::setupShaderProgram() {
    const char *const vertex_source = 
        "Placeholder";
    
    const char *const fragment_source =
        "Placeholder";

    GLint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragment_source);
    GLint vertexShader = compileShader(GL_VERTEX_SHADER, vertex_source);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    //glBindAttribLocation(shaderProgram, 0, "position");

    glLinkProgram(shaderProgram);
    glValidateProgram(shaderProgram);
}

GLuint Renderer::compileShader (GLenum type, std::string source) {

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
        printf("%s\n", infoLog);

    }

    return shader;
}

void Renderer::drawTriangleFan(std::vector<std::tuple<double,double>> points, Color col) {}