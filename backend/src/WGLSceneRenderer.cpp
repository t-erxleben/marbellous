#include "WGLSceneRenderer.hpp"

WGLSceneRenderer::WGLSceneRenderer()
{
	auto dropRes = WGLContext::getContext()->getDropRes();
	

    setupShaderProgram(vertex_source, fragment_source, shaderProgram);

    // init vbo
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // init ebo
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    // define position
    GLint pos = glGetAttribLocation(shaderProgram, "position");
    GLint col = glGetAttribLocation(shaderProgram, "colorCode");
	GLint z   = glGetAttribLocation(shaderProgram, "z");
	char* offset = 0;

	//define attribs
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
    colorLoc = glGetUniformLocation(shaderProgram, "c");
	drawColorLoc = glGetUniformLocation(shaderProgram, "drawColor");
	locDisR2  = glGetUniformLocation(shaderProgram, "disR2");
	locDisP   = glGetUniformLocation(shaderProgram, "disP");

    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glGenTextures(1, &frameTexture);
    glBindTexture(GL_TEXTURE_2D, frameTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dropRes, dropRes, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameTexture, 0);
	
	glGenTextures(1, &depthStencilTexture);
	glBindTexture(GL_TEXTURE_2D, depthStencilTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, dropRes, dropRes, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthStencilTexture, 0);

    GLenum drawBuffers[2] = {GL_COLOR_ATTACHMENT0, GL_DEPTH_STENCIL_ATTACHMENT};
    glDrawBuffers(1, drawBuffers);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        fprintf(stderr, "failed to create framebuffer!");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
	glClearDepthf(1.);
	glClearStencil(0b10);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_DEPTH_TEST);
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
    for (auto itr = scene.rbegin(); itr != scene.rend(); ++itr )
    {
		const Polygon& p = *itr;
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

void WGLSceneRenderer::setActive() const
{
	auto dropRes = WGLContext::getContext()->getDropRes();

    glUseProgram(shaderProgram);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    // define position
    GLint pos = glGetAttribLocation(shaderProgram, "position");
    GLint col = glGetAttribLocation(shaderProgram, "colorCode");
	GLint z   = glGetAttribLocation(shaderProgram, "z");
	char* offset = 0;

	//define attribs
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

	glClearDepthf(1.);
	glClearStencil(0b10);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_DEPTH_TEST);

    // Set the viewport
    glViewport(0, 0, dropRes, dropRes);
}

void WGLSceneRenderer::drawToBuffer(const Scene &scene, char *data, int len, bool drawColor)
{
	auto dropRes = WGLContext::getContext()->getDropRes();

    assert(len == dropRes * dropRes * 4);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glViewport(0, 0, dropRes, dropRes);
    drawScene(scene, drawColor);
    glReadPixels(0, 0, dropRes, dropRes, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void WGLSceneRenderer::drawScene(Scene const &scene, bool drawColor)
{
	setActive();
	
	WGLVertex* vertices = nullptr;
	GLuint* indices = nullptr;
	if (generation != scene.getGeneration()) {
		generation = scene.getGeneration();
		constructBuffers(&indices, &vertices, scene, i_size, v_size);
		glBufferData(GL_ARRAY_BUFFER, v_size, vertices, GL_DYNAMIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, i_size, indices, GL_DYNAMIC_DRAW);
	}

    // set uniforms
    auto opt = Options::getInstance();
    const auto& p = *opt->getActivePalette();

	// set colors
	std::vector<GLfloat> v;
	buildColorBuffer(p, v);
	glUniform3fv(colorLoc, p.getSize(), v.data());

	// set draw mode
	glUniform1i(drawColorLoc, drawColor);
	
	const auto& diss = scene.getDisplacements(); // TODO
	if(diss.empty()) {
		glUniform1f(locDisR2, 0);
		glUniform2f(locDisP, 0, 0);
	} else {
		const auto& dis = diss[0];
		glUniform1f(locDisR2, dis.r*dis.r);
		glUniform2f(locDisP, dis.p.x, dis.p.y);
	}
	if(!drawColor) glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glDepthMask(GL_TRUE);
	glStencilMask(0b11);
	glStencilFunc(GL_NOTEQUAL, 0b00, 0b11);
	glStencilOp(GL_KEEP, GL_INVERT, GL_DECR);
	glDepthFunc(GL_NOTEQUAL);
	glDrawElements(GL_TRIANGLE_FAN, i_size / sizeof(GLuint), GL_UNSIGNED_INT, 0);

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glStencilFunc(GL_EQUAL, 0b00, 0b11);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_FALSE);
	glDrawElements(GL_TRIANGLE_FAN, i_size / sizeof(GLuint), GL_UNSIGNED_INT, 0);

	glStencilFunc(GL_EQUAL, 0b01, 0b01);
	glDepthFunc(GL_EQUAL);
	glDrawElements(GL_TRIANGLE_FAN, i_size / sizeof(GLuint), GL_UNSIGNED_INT, 0);

	if(indices) { delete[] indices; }
	if(vertices) { delete[] vertices; }
	auto [r, g, b] = Options::getInstance()->getBGColor()->getRGB();
	if(!drawColor) { glClearColor(r/256.f, g/256.f, b/256.f, 1.); }
}
