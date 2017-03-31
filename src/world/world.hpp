#pragma once

#include <vector>
#include <memory>

#include "component/component.hpp"
#include "entity/entity.hpp"

class World {
public:
	World();
	virtual ~World();

	World& addEntity(std::shared_ptr<Entity> entity);

	void update(float delta);
	void render();

private:
	std::vector<std::shared_ptr<Entity>> _entities;
};