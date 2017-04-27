// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "physicssystem.hpp"

#include "../component/transformcomponent.hpp"
#include "../component/physicscomponent.hpp"

PhysicsSystem::~PhysicsSystem() {}

void PhysicsSystem::update(World& world, float delta) {
	rmt_ScopedCPUSample(PhysicsSystem, RMTSF_None);
	// TODO: Compute shader this?

	for (std::unique_ptr<Entity>& entity : world.getEntities()) {
		auto physicsComponent = entity->getComponent<PhysicsComponent>();
		if (!physicsComponent)
			continue;

		auto transformComponent = entity->getComponent<TransformComponent>();
		if (!transformComponent)
			continue;

		glm::vec3 deltaPos = physicsComponent->velocity * delta + physicsComponent->acceleration * delta * delta * 0.5f;
		transformComponent->move(deltaPos);

		physicsComponent->velocity += physicsComponent->acceleration * delta;
	}
}

void PhysicsSystem::registerImGui() {}
