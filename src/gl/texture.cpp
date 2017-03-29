// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "texture.hpp"

Texture::Texture(const std::string& texture) {
	SDL_Surface* surface = IMG_Load(texture.c_str());
	if (!surface)
		throw "Texture failed to load!";

	GLenum format;

	int nOfColors = surface->format->BytesPerPixel;
	if (nOfColors == 4) {
		if (surface->format->Rmask == 0x000000ff)
			format = GL_RGBA;
		else
			format = GL_BGRA;
	} else if (nOfColors == 3) {
		if (surface->format->Rmask == 0x000000ff)
			format = GL_RGB;
		else
			format = GL_BGR;
	} else {
		SDL_Surface* newSurf = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGB24, 0);
		if (!newSurf)
			throw "Unknown texture format";
		SDL_FreeSurface(surface);
		surface = newSurf;
		format = GL_RGB;
	}

	_setData(format, surface->w, surface->h, surface->pixels);

	SDL_FreeSurface(surface);
}

Texture::Texture(GLuint texture) : _texture(texture) {}

Texture::Texture(unsigned int width, unsigned int height, const void* data) {
	_setData(GL_RGBA, width, height, data);
}
Texture::Texture(const char imageFormat[4], const void* data, unsigned int size) {
	SDL_Surface* surface = IMG_LoadTyped_RW(SDL_RWFromConstMem(data, size), 1, imageFormat);
	if (!surface)
		throw "Texture failed to load!";

	GLenum format;

	int nOfColors = surface->format->BytesPerPixel;
	if (nOfColors == 4) {
		if (surface->format->Rmask == 0x000000ff)
			format = GL_RGBA;
		else
			format = GL_BGRA;
	} else if (nOfColors == 3) {
		if (surface->format->Rmask == 0x000000ff)
			format = GL_RGB;
		else
			format = GL_BGR;
	} else {
		SDL_Surface* newSurf = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGB24, 0);
		if (!newSurf)
			throw "Unknown texture format";
		SDL_FreeSurface(surface);
		surface = newSurf;
		format = GL_RGB;
	}

	_setData(format, surface->w, surface->h, surface->pixels);

	SDL_FreeSurface(surface);
}

Texture::~Texture() {
	glDeleteTextures(1, &_texture);
}

void Texture::bind(int slot) {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, _texture);
}

GLuint Texture::getTexture() {
	return _texture;
}

void Texture::_setData(GLenum format, GLuint w, GLuint h, const void* pixels) {
	glGenTextures(1, &_texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, format, GL_UNSIGNED_BYTE, pixels);
}
