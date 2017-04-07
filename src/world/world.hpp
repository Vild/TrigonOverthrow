#pragma once

#include <vector>
#include <memory>

class Entity;
class System;

class World {
public:
	World();

	template <typename T>
	std::shared_ptr<Entity> addEntity(std::shared_ptr<T> entity) {
		std::shared_ptr<Entity> ent = entity;
		_entities.push_back(ent);
		return ent;
	}

	void tick(float delta);

	inline std::vector<std::shared_ptr<Entity>>& getEntities() { return _entities; }

private:
	std::vector<std::shared_ptr<Entity>> _entities;
	std::vector<std::unique_ptr<System>> _systems;

	void _setupSystems();
};

#include "system/system.hpp"
#include "entity/entity.hpp"