#pragma once
#include <map>
#include <memory>
#include <stdio.h>
#include <string>
#include <vector>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

class MapLoader {
public:
	MapLoader();
	virtual ~MapLoader();
	std::vector<Uint8> getMap(const std::string& filename);
	int getWidth() { return _width; }
	int getHeight() { return _height; }

private:
	SDL_Surface* _imageData;
	int _width, _height;
	std::vector<Uint8> _data;
};