#include "WGLSceneRenderer.hpp"

WGLSceneRenderer::WGLSceneRenderer()
{
    setupShaderProgram(vertex_source, fragment_source);
}

void WGLSceneRenderer::constructBuffers(void **indices, void **vert, Scene const &scene)
{
    std::vector<GLuint> i_vec{};
    size_t v_size = 0;
    // todo: maybe save total vertex count in scene to avoid this loop
    for (Polygon const p : scene)
    {
        v_size += p.getVertCount() * sizeof(WGLVertex);
    }
    *vert = malloc(v_size);

    char *v_pos = (char *)*vert;
    for (Polygon const p : scene)
    {
        std::vector<GLuint> i_loc_vec{};
        std::vector<WGLVertex> v_vec{};
        p.getDrawInfo(i_loc_vec, v_vec);
        i_vec.insert(i_vec.end(), i_loc_vec.begin(), i_loc_vec.end());

        // cpy memory
        memcpy(v_pos, v_vec.data(), v_vec.size() * sizeof(WGLVertex));

        // advance pos
        v_pos += v_vec.size() * sizeof(WGLVertex);
    }

    *indices = malloc(i_vec.size() * sizeof(GLuint));
    memcpy(*indices, i_vec.data(), i_vec.size() * sizeof(GLuint));
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