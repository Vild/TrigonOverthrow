#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../lib/glad.h"
#include <string>

class Texture {
public:
	Texture(const std::string& texture);
	Texture(GLuint texture);
	Texture(unsigned int width, unsigned int height, const void* data);
	Texture(const char imageFormat[4], const void* data, unsigned int size);
	virtual ~Texture();

	void bind(int slot);

	void resize(unsigned int width, unsigned int height, GLenum internalFormat, GLenum dataFormat, GLenum dataType);

	GLuint getTexture();

private:
	GLuint _texture;

	void _setData(GLenum format, GLuint w, GLuint h, const void* pixels);
};
