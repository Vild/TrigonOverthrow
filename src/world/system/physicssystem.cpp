#include "physicssystem.hpp"

#include "../component/transformcomponent.hpp"
#include "../component/physicscomponent.hpp"

void PhysicsSystem::update(World& world, float delta) {
	// TODO: Compute shader this?

	for (std::shared_ptr<Entity> entity : world.getEntities())
	{
		auto physicsComponent = entity->getComponent<PhysicsComponent>();
		if (!physicsComponent)
			continue;

		auto transformComponent = entity->getComponent<TransformComponent>();
		if (!transformComponent)
			continue;

		transformComponent->position += physicsComponent->velocity * delta + physicsComponent->acceleration * delta * delta * 0.5f;
		transformComponent->recalculateMatrix();

		physicsComponent->velocity += physicsComponent->acceleration * delta;
	}
}
