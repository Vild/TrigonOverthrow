#pragma once

#include "../lib/glad.h"
#include <map>
#include <memory>

#include "texture.hpp"

class GBuffer {
public:
	GBuffer();
	GBuffer(GLuint fb);
	virtual ~GBuffer();

	GBuffer& bind(bool read = true, bool draw = true);
	GBuffer& attachTexture(int id, std::shared_ptr<Texture> texture);
	GBuffer& attachTexture(int id, size_t width, size_t height, GLenum internalFormat, GLenum dataType, int vectorSize);
	GBuffer& attachRenderBuffer(size_t width, size_t height, GLenum format = GL_DEPTH24_STENCIL8, GLenum attachment = GL_DEPTH_STENCIL_ATTACHMENT);
	GBuffer& attachDepthTexture(int id, size_t width, size_t height);

	GBuffer& finalize();

	inline std::map<GLuint, std::shared_ptr<Texture>>& getAttachments() { return _attachments; }

	std::shared_ptr<Texture>& getAttachment(int id);

private:
	GLuint _fb;
	std::map<GLuint, std::shared_ptr<Texture>> _attachments;
	GLuint _renderBuffer;
};
