// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "entity.hpp"

#include <cstdio>

Entity::Entity(World& world, std::string name) : _world(world), _name(name) {}

Entity::~Entity() {
	for (auto& pair : components) {
		try {
			components.erase(pair.first);
			auto& ents = _world._activeComponents[pair.first];
			auto it = std::find(ents.begin(), ents.end(), this);
			ents.erase(it);
		} catch (std::exception&) {
			fprintf(stderr, "MEMORY LEAK!!! Component failed being freed!");
		}
	}
}