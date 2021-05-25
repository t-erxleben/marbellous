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
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLushort), 0);
    glEnableVertexAttribArray(0);
    // define color
    glVertexAttribPointer(1, 1, GL_UNSIGNED_SHORT, GL_FALSE, 2*sizeof(GLfloat), (void*) (2*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // lookup uniform location
    color0Loc = glGetUniformLocation(shaderProgram, "c0");
    color1Loc = glGetUniformLocation(shaderProgram, "c1");
    color2Loc = glGetUniformLocation(shaderProgram, "c2");
    color3Loc = glGetUniformLocation(shaderProgram, "c3");
}

void WGLSceneRenderer::constructBuffers(void **indices, void **vertices, Scene const &scene, size_t & indices_size, size_t & vertices_size)
{
    std::vector<std::vector<WGLVertex>*> all_vertices{scene.getPolygonCount()};
    std::vector<std::vector<GLuint>*> all_indices{scene.getPolygonCount()};

    size_t v_count = 0;
    size_t i_count = 0;

    for(int i = 0; i < scene.getPolygonCount(); ++i)
    {
        scene[i].getDrawInfo(all_indices[i], all_vertices[i]);
        v_count+=all_vertices[i]->size();
        i_count+=all_indices[i]->size();
    }

    indices_size = i_count * sizeof(GLuint);
    vertices_size = v_count * sizeof(WGLVertex);

    *indices = malloc(indices_size);
    *vertices = malloc(vertices_size);

    size_t pos = 0;

    for(auto i : all_indices)
    {
        // Translate indices into index space of all triangles for all polygons
        for(auto entry: *i) entry += pos;
        memcpy((GLuint*)*indices + pos, i->data(), i->size() * sizeof(GLuint));
        pos += i->size();
    }

    pos = 0;

    for(auto v : all_vertices)
    {
        memcpy((GLuint*)*vertices + pos, v->data(), v->size() * sizeof(WGLVertex));
        pos += v->size();
    }
}

void WGLSceneRenderer::setActive()
{
    glUseProgram(shaderProgram);
    glBindBuffer(GL_ARRAY_BUFFER, vao);
}

void WGLSceneRenderer::drawScene(Scene const &scene)
{
    setActive();
    
    void* indices, *vertices;
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

    glUniform3i(color0Loc, std::get<0>(c0)/255.0f, std::get<1>(c0)/255.0f, std::get<2>(c0)/255.0f);
    glUniform3i(color1Loc, std::get<0>(c1)/255.0f, std::get<1>(c1)/255.0f, std::get<2>(c1)/255.0f);
    glUniform3i(color2Loc, std::get<0>(c2)/255.0f, std::get<1>(c2)/255.0f, std::get<2>(c2)/255.0f);
    glUniform3i(color3Loc, std::get<0>(c3)/255.0f, std::get<1>(c3)/255.0f, std::get<2>(c3)/255.0f);

    // opengl es only supports GL_UNSIGNED_SHORT????!
    glDrawElements(GL_TRIANGLES, i_size/sizeof(GLuint), GL_UNSIGNED_INT, 0);
}