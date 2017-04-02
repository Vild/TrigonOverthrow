#include "world.hpp"

World& World::addEntity(std::shared_ptr<Entity> entity) {
	_entities.push_back(entity);

	return *this;
}
