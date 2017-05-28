// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "entity.hpp"

#include <cstdio>

Entity::Entity(World& world, std::string name) : _world(world), _name(name) {}

Entity::~Entity() {
	auto it = _components.begin();
	while (it != _components.end()) {
		auto& ents = _world._activeComponents[(*it).first];
		ents.erase(std::find(ents.begin(), ents.end(), this));
		it++;
	}
}
