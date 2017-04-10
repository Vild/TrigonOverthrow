#pragma once

#include <vector>
#include <memory>

class Entity;
class System;

class World {
public:
	World();

	template <typename T, typename std::enable_if<std::is_base_of<Entity, T>::value>::type* = nullptr>
	std::shared_ptr<Entity> addEntity(std::shared_ptr<T> entity) {
		std::shared_ptr<Entity> ent = entity;
		_entities.push_back(ent);
		return ent;
	}

	void tick(float delta);
	void resize(unsigned int width, unsigned int height);

	inline std::vector<std::shared_ptr<Entity>>& getEntities() { return _entities; }
	inline std::vector<std::unique_ptr<System>>& getSystems() { return _systems; }

private:
	std::vector<std::shared_ptr<Entity>> _entities;
	std::vector<std::unique_ptr<System>> _systems;

	void _setupSystems();
};

#include "system/system.hpp"
#include "entity/entity.hpp"
