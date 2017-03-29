// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "gbuffer.hpp"

#include <iostream>

GBuffer::GBuffer() {
	glGenFramebuffers(1, &_fb);
	_renderBuffer = 0;
}

GBuffer::GBuffer(GLuint fb) : _fb(fb), _renderBuffer(0) {}

GBuffer::~GBuffer() {
	if (_renderBuffer)
		glDeleteRenderbuffers(1, &_renderBuffer);

	_attachments.clear();

	// I use a framebuffer object with the id zero for the screen.
	// So don't try and remove that one. OpenGL will (probably) be mad if you do that.
	if (_fb)
		glDeleteFramebuffers(1, &_fb);
}

GBuffer& GBuffer::bind(bool read, bool draw) {
	if (read) {
		if (draw)
			glBindFramebuffer(GL_FRAMEBUFFER, _fb);
		else
			glBindFramebuffer(GL_READ_FRAMEBUFFER, _fb);
	} else
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fb);

	return *this;
}

GBuffer& GBuffer::attachTexture(int id, size_t width, size_t height, GLenum dataFormat, GLenum dataType, int vectorSize) {
	GLuint texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	GLenum formats[] = {GL_RED, GL_RG, GL_RGB, GL_RGBA};

	glTexImage2D(GL_TEXTURE_2D, 0, dataFormat, width, height, 0, formats[vectorSize - 1], dataType, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + id, texID, 0);

	_attachments.push_back(attachment{id, std::make_shared<Texture>(texID)});

	return *this;
}

GBuffer& GBuffer::attachDepthTexture(int id, size_t width, size_t height) {
	GLuint depthTexture;
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	GLfloat border[4] = {1, 0, 0, 0};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	_attachments.push_back(attachment{id, std::make_shared<Texture>(depthTexture)});

	return *this;
}

GBuffer& GBuffer::attachRenderBuffer(size_t width, size_t height, GLenum format) {
	glGenRenderbuffers(1, &_renderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, _renderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _renderBuffer);
	return *this;
}

GBuffer& GBuffer::finalize() {
	std::vector<GLenum> buffers;
	if (_attachments.size()) {
		for (size_t i = 0; i < _attachments.size(); i++)
			buffers.push_back(GL_COLOR_ATTACHMENT0 + _attachments[i].id);
		glDrawBuffers(buffers.size(), &buffers[0]);
	}

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "Framebuffer failed!" << std::endl << "\tStatus: " << status << std::endl;
		throw new std::exception();
	}
	return *this;
}

const std::vector<attachment>& GBuffer::getAttachments() {
	return _attachments;
}
