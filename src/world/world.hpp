#pragma once

#include <vector>
#include <memory>

class Entity;

class World {
public:
	World();

	void addEntity(std::shared_ptr<Entity> entity) { _entities.push_back(entity); }

	inline std::vector<std::shared_ptr<Entity>>& getEntities() { return _entities; }

private:
	std::vector<std::shared_ptr<Entity>> _entities;
};

#include "entity.hpp"
