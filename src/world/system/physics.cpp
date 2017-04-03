#include "physics.hpp"

#include "../component/transform.hpp"

void PhysicsSystem::update(World& world, float delta) {
	//TODO: Compute shader this?
	for (std::shared_ptr<TransformComponent> transform : TransformComponent::getActiveComponents()) {
		transform->position += transform->velocity * delta + (transform->acceleration * delta * delta) / 2.0f;
		transform->velocity += transform->acceleration * delta;
	}
}
