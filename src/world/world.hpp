#pragma once

#include <vector>
#include <memory>

#include "component/component.hpp"
#include "entity/entity.hpp"

class World {
public:
	World& addEntity(std::shared_ptr<Entity> entity);

	inline std::vector<std::shared_ptr<Entity>>& getEntities() { return _entities; }

private:
	std::vector<std::shared_ptr<Entity>> _entities;
};