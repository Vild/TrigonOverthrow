#include "world.hpp"

#include "component/render.hpp"

World::World() {
}

World::~World() {

}

World& World::addEntity(std::shared_ptr<Entity> entity) {
	_entities.push_back(entity);

	return *this;
}

void World::update(float delta) {
	for (std::shared_ptr<Entity> e : _entities)
		e->update(delta);
}

void World::render() {
	for (std::shared_ptr<RenderComponent> r : RenderComponent::getActiveComponents())
		r->render();
}