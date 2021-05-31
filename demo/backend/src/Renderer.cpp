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
    attrs.preserveDrawingBuffer = 1;

    context = emscripten_webgl_create_context(canvasID.c_str(), &attrs);
    emscripten_webgl_make_context_current(context);
	glClearColor(0xF7 / 255.f, 0xE9 / 255.f, 0xCE / 255.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::setupShaderProgram() {
    const char *const vertex_source = 
        "attribute vec2 position; void main(){gl_Position = vec4(position, 0.0, 1.0);}\n";
    
    const char *const fragment_source =
        "precision mediump float; uniform vec3 color; void main(){gl_FragColor = vec4(color, 1.0);}\n";

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

void Renderer::drawTriangleFan(GLfloat* points, size_t pointCount, Color col)
{
    // activate shader and lookup uniforms
    glUseProgram(shaderProgram);
    GLint positionLoc = glGetAttribLocation(shaderProgram, "position");
    GLint colorLoc = glGetUniformLocation(shaderProgram, "color");

    // load triangle data
    GLuint vertexObject;
    glGenBuffers(1, &vertexObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexObject);
    glBufferData(GL_ARRAY_BUFFER, pointCount * 2 * sizeof(GLfloat), (void*)points, GL_STATIC_DRAW);

    glVertexAttribPointer(positionLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0); 

    auto rgb = col.getRGB();

    GLfloat r = static_cast<float>(std::get<0>(rgb)) / 255.f;
    GLfloat g = static_cast<float>(std::get<1>(rgb)) / 255.f;
    GLfloat b = static_cast<float>(std::get<2>(rgb)) / 255.f;

    glUniform3f(colorLoc, r, g, b);

    glDrawArrays(GL_TRIANGLE_FAN, 0, pointCount);
}
