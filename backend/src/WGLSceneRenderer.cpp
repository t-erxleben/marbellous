#include "WGLSceneRenderer.hpp"

WGLSceneRenderer::WGLSceneRenderer()
{
    setupShaderProgram(vertex_source, fragment_source);

    // init vao
    glGenBuffers(1, &vao);
    glBindBuffer(GL_ARRAY_BUFFER, vao);

    // init ebo
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    // define attribs for vao
    // define position
    GLint pos = glGetAttribLocation(shaderProgram, "position");
    GLint col = glGetAttribLocation(shaderProgram, "colorCode");
    glVertexAttribPointer(pos, 2, GL_FLOAT, GL_FALSE, sizeof(WGLVertex), 0);
    glEnableVertexAttribArray(pos);
    // define color
    glVertexAttribIPointer(col, 1, GL_UNSIGNED_INT, sizeof(WGLVertex), (void *)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(col);

    // lookup uniform location
    color0Loc = glGetUniformLocation(shaderProgram, "c0");
    color1Loc = glGetUniformLocation(shaderProgram, "c1");
    color2Loc = glGetUniformLocation(shaderProgram, "c2");
    color3Loc = glGetUniformLocation(shaderProgram, "c3");

    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glGenTextures(1, &frameTexture);
    glBindTexture(GL_TEXTURE_2D, frameTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 720, 720, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameTexture, 0);
    GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, drawBuffers);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        fprintf(stderr, "failed to create framebuffer!");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void WGLSceneRenderer::constructBuffers(void **indices, void **vertices, Scene const &scene, size_t &indices_size, size_t &vertices_size)
{
    std::vector<std::vector<WGLVertex>> all_vertices{scene.getPolygonCount()};
    std::vector<std::vector<GLuint>> all_indices{scene.getPolygonCount()};

    size_t v_count = 0;
    size_t i_count = 0;

    for (int i = 0; i < scene.getPolygonCount(); ++i)
    {
        scene[i].getDrawInfo(&all_indices[i], &all_vertices[i]);
        v_count += all_vertices[i].size();
        i_count += all_indices[i].size();
    }

    indices_size = i_count * sizeof(GLuint);
    vertices_size = v_count * sizeof(WGLVertex);

    *indices = malloc(indices_size);
    *vertices = malloc(vertices_size);

    size_t pos = 0;
    size_t offset = 0;
    for (int itr = 0; itr < scene.getPolygonCount(); ++itr)
    {
        // Translate indices into index space of all triangles for all polygons
        auto &i = all_indices[itr];
        auto &v = all_vertices[itr];

        for (auto &entry : i)
            entry += offset;

        memcpy(static_cast<WGLVertex *>(*vertices) + offset, v.data(), v.size() * sizeof(WGLVertex));
        offset += v.size();

        memcpy(static_cast<GLuint *>(*indices) + pos, i.data(), i.size() * sizeof(GLuint));
        pos += i.size();
    }
}

void WGLSceneRenderer::setActive()
{
    glUseProgram(shaderProgram);
    glBindBuffer(GL_ARRAY_BUFFER, vao);

    // Set the viewport
    glViewport(0, 0, 720, 720);
}

void WGLSceneRenderer::drawToBuffer(const Scene &scene, char *data, int len)
{
    assert(len == 720 * 720 * 4);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glViewport(0, 0, 720, 720);
    drawScene(scene);
    glReadPixels(0, 0, 720, 720, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void WGLSceneRenderer::drawScene(Scene const &scene)
{
    setActive();

    void *indices, *vertices;
    size_t i_size, v_size;

    constructBuffers(&indices, &vertices, scene, i_size, v_size);

    glBufferData(GL_ARRAY_BUFFER, v_size, vertices, GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, i_size, indices, GL_DYNAMIC_DRAW);

    // todo set uniforms
    auto opt = Options::getInstance();
    auto p = opt->getActivePalette();
    auto c0 = (*p)[0].getRGB();
    auto c1 = (*p)[1].getRGB();
    auto c2 = (*p)[2].getRGB();
    auto c3 = (*p)[3].getRGB();

    glUniform3f(color0Loc, std::get<0>(c0) / 255.0f, std::get<1>(c0) / 255.0f, std::get<2>(c0) / 255.0f);
    glUniform3f(color1Loc, std::get<0>(c1) / 255.0f, std::get<1>(c1) / 255.0f, std::get<2>(c1) / 255.0f);
    glUniform3f(color2Loc, std::get<0>(c2) / 255.0f, std::get<1>(c2) / 255.0f, std::get<2>(c2) / 255.0f);
    glUniform3f(color3Loc, std::get<0>(c3) / 255.0f, std::get<1>(c3) / 255.0f, std::get<2>(c3) / 255.0f);

    glClear(GL_COLOR_BUFFER_BIT);
    // opengl es only supports GL_UNSIGNED_SHORT????!
    glDrawElements(GL_TRIANGLES, i_size / sizeof(GLuint), GL_UNSIGNED_INT, 0);

    free(indices);
    free(vertices);
}
