#pragma once

#include "../lib/glad.h"
#include <vector>
#include <memory>

#include "texture.hpp"

struct attachment {
	int id;
	std::shared_ptr<Texture> texture;
};

class GBuffer {
public:
	GBuffer();
	GBuffer(GLuint fb);
	virtual ~GBuffer();

	GBuffer& bind(bool read = true, bool draw = true);

	GBuffer& attachTexture(int id, size_t width, size_t height, GLenum dataFormat, GLenum dataType, int vectorSize);
	GBuffer& attachRenderBuffer(size_t width, size_t height, GLenum format = GL_DEPTH24_STENCIL8);
	GBuffer& attachDepthTexture(int id, size_t width, size_t height);

	GBuffer& finalize();

	const std::vector<attachment>& getAttachments();

private:
	GLuint _fb;
	std::vector<attachment> _attachments;
	GLuint _renderBuffer;
};
