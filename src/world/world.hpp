#pragma once

#include <vector>
#include <memory>
#include <map>
#include <typeindex>

class Entity;

class World {
public:
	World();
	virtual ~World();

	Entity* addEntity(const std::string& name = "Generic") {
		_entities.push_back(std::make_unique<Entity>(*this, name));
		return _entities.back().get();
	}

	inline std::vector<std::unique_ptr<Entity>>& getEntities() { return _entities; }

	template <typename T>
	std::vector<Entity*>& getActiveComponents() {
		return _activeComponents[typeid(T)];
	}

private:
	friend class Entity;
	std::vector<std::unique_ptr<Entity>> _entities;
	std::map<std::type_index, std::vector<Entity*>> _activeComponents;
};

#include "entity.hpp"
