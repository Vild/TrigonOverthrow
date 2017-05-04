// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "maploader.hpp"

MapLoader::MapLoader() {
	_width = _height = 0;
	_imageData = nullptr;
}

MapLoader::~MapLoader() {}

std::vector<Uint8> MapLoader::getMap(const std::string& filename) {
	// Don't need _data member variable. Might have a list of maps instead.

	SDL_Surface* loadedSurface = IMG_Load(filename.c_str());
	if (loadedSurface == NULL)
		printf("Couldn't load image %s! SDL_image error: %s\n", filename.c_str(), IMG_GetError());

	int bpp = loadedSurface->format->BytesPerPixel;

	_width = loadedSurface->w;
	_height = loadedSurface->h;
	for (int y = _height - 1; y >= 0; y--)
		for (int x = 0; x < _width; x++)
			_data.push_back(((Uint8*)loadedSurface->pixels)[loadedSurface->pitch * y + x * bpp]);

	SDL_FreeSurface(loadedSurface);
	return _data;
}
