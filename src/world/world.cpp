// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "world.hpp"

World::World() {}

World::~World() {
	_entities.clear();
	_activeComponents.clear();
}