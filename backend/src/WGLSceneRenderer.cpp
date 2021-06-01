#include "WGLSceneRenderer.hpp"

#include <cassert>

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
	GLint z   = glGetAttribLocation(shaderProgram, "z");
	char* offset = 0;

    glVertexAttribPointer(pos, 2, GL_FLOAT, GL_FALSE, sizeof(WGLVertex), offset);
    glEnableVertexAttribArray(pos);
	offset += sizeof(WGLVertex::p);

    glVertexAttribIPointer(z, 1, GL_UNSIGNED_INT, sizeof(WGLVertex), offset);
    glEnableVertexAttribArray(z);
	offset += sizeof(WGLVertex::z);

    // define color
    glVertexAttribIPointer(col, 1, GL_UNSIGNED_INT, sizeof(WGLVertex), offset);
    glEnableVertexAttribArray(col);
	offset += sizeof(WGLVertex::color);

	assert(offset == (char*)sizeof(WGLVertex));

    // lookup uniform location
    color0Loc = glGetUniformLocation(shaderProgram, "c0");
    color1Loc = glGetUniformLocation(shaderProgram, "c1");
    color2Loc = glGetUniformLocation(shaderProgram, "c2");
    color3Loc = glGetUniformLocation(shaderProgram, "c3");
	locDisR2  = glGetUniformLocation(shaderProgram, "disR2");
	locDisP   = glGetUniformLocation(shaderProgram, "disP");

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

void WGLSceneRenderer::constructBuffers(GLuint **indices, WGLVertex **vertices, Scene const &scene, size_t &indices_size, size_t &vertices_size)
{
    size_t v_count = 0;
	for(const auto& p : scene) { v_count += p.getVertCount(); }
	size_t i_count = v_count + scene.getPolygonCount();

    vertices_size = v_count * sizeof(WGLVertex);
	indices_size = i_count * sizeof(GLuint);

    WGLVertex* verts = new WGLVertex[v_count];
	*vertices = verts;
	*indices = new GLuint[i_count];

    size_t v_offset = 0;
    size_t i_offset = 0;
	std::vector<WGLVertex> verts_v;
	GLuint count =  0;
    for (const auto& p : scene)
    {
		p.getDrawInfo(verts_v, count);
		memcpy(verts + v_offset, verts_v.data(), verts_v.size() * sizeof(WGLVertex));
		for(size_t i = 0; i < verts_v.size(); ++i) {
			(*indices)[i_offset + i] = v_offset+i;
		}
		(*indices)[i_offset + verts_v.size()] = PRIMITIVE_RESTART;
		i_offset += verts_v.size() + 1;
		v_offset += verts_v.size();
		++count;
		assert(count != 0); // overflow check
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
	glEnable(GL_STENCIL_TEST);

	WGLVertex* vertices = nullptr;
	GLuint* indices = nullptr;
	if (generation != scene.getGeneration()) {
		generation = scene.getGeneration();
		constructBuffers(&indices, &vertices, scene, i_size, v_size);
		glBufferData(GL_ARRAY_BUFFER, v_size, vertices, GL_DYNAMIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, i_size, indices, GL_DYNAMIC_DRAW);
	}

    // todo set uniforms
    auto opt = Options::getInstance();
    const auto& p = *opt->getActivePalette();
	GLint locs[] = {color0Loc, color1Loc, color2Loc, color3Loc};
	for(size_t i = 0; i < p.getSize(); ++i) {
		auto c = p[i].getRGB();
		glUniform3f(locs[i], std::get<0>(c) / 255.0f, std::get<1>(c) / 255.0f, std::get<2>(c) / 255.0f);
	}

	const auto& dis = scene.getDisplacement();
	glUniform1f(locDisR2, dis.r*dis.r);
	glUniform2f(locDisP, dis.p.x, dis.p.y);

    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glStencilFunc(GL_EQUAL, 1, 0x01);
	glDepthFunc(GL_LESS);
	glStencilOp(GL_INVERT, GL_KEEP, GL_INCR);
	glStencilMask(0xFF);

    glDrawElements(GL_TRIANGLE_FAN, i_size / sizeof(GLuint), GL_UNSIGNED_INT, 0);

	glStencilFunc(GL_EQUAL, 1,  0x01);
	glDepthFunc(GL_ALWAYS);
	glStencilMask(0x00);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    glDrawElements(GL_TRIANGLE_FAN, i_size / sizeof(GLuint), GL_UNSIGNED_INT, 0);

	if(indices) { delete[] indices; }
	if(vertices) { delete[] vertices; }
}
