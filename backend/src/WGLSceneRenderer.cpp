#include "WGLSceneRenderer.hpp"

WGLSceneRenderer::WGLSceneRenderer()
{
    setupShaderProgram(vertex_source, fragment_source);
}

void WGLSceneRenderer::constructBuffers(void **indices, void **vertices, Scene const &scene)
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

    *indices = malloc(i_count * sizeof(GLuint));
    *vertices = malloc(v_count * sizeof(WGLVertex));

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
}

void WGLSceneRenderer::drawScene(Scene const &scene)
{
    setActive();
    //todo
}