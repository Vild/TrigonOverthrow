#pragma once

#include <vector>
#include <memory>
#include "../lib/sole/sole.hpp"

class Entity;

class World {
public:
	World();
	virtual ~World();

	Entity* addEntity(const sole::uuid& uuid = sole::uuid4(), const std::string& name = "Generic") {
		_entities.push_back(std::make_unique<Entity>(uuid, name));
		return _entities.back().get();
	}

	inline std::vector<std::unique_ptr<Entity>>& getEntities() { return _entities; }

private:
	std::vector<std::unique_ptr<Entity>> _entities;
};

#include "entity.hpp"
