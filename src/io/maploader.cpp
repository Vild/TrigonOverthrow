// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "maploader.hpp"

MapLoader::MapLoader() {}

MapLoader::~MapLoader() {}

MapData MapLoader::loadFromImage(const std::string& filename) {
	// Don't need _data member variable. Might have a list of maps instead.

	SDL_Surface* loadedSurface = IMG_Load(filename.c_str());
	if (loadedSurface == NULL)
		throw printf("Couldn't load image %s! SDL_image error: %s\n", filename.c_str(), IMG_GetError());

	int bpp = loadedSurface->format->BytesPerPixel;

	int width = loadedSurface->w;
	int height = loadedSurface->h;
	std::vector<Uint8> data;

	for (int y = height - 1; y >= 0; y--)
		for (int x = 0; x < width; x++)
			data.push_back(((Uint8*)loadedSurface->pixels)[loadedSurface->pitch * y + x * bpp]);

	SDL_FreeSurface(loadedSurface);
	return { width, height, data };
}
