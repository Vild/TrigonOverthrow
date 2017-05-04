#pragma once
#include <map>
#include <memory>
#include <stdio.h>
#include <string>
#include <vector>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

struct MapData
{
	int width;
	int height;
	std::vector<Uint8> data;
};

class MapLoader {
public:
	MapLoader();
	virtual ~MapLoader();
	MapData loadFromImage(const std::string& filename);
};