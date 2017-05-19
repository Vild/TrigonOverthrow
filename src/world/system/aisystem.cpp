#include <cmath>
#include "aisystem.hpp"
#include "../component/rigidbodycomponent.hpp"
#include "../component/transformcomponent.hpp"
#include "../component/aicomponent.hpp"
#define PI 3.14159265

void AISystem::update(World& world, float delta) {
	for (std::unique_ptr<Entity>& entity : world.getEntities()) {
		auto ai = entity->getComponent<AIComponent>();
		if (!ai)
			continue;
		auto rdbComp = entity->getComponent<RigidBodyComponent>();
		auto rigidbody = rdbComp->getRigidBody();
		auto dir = entity->getComponent<TransformComponent>()->getDirection();
		if (ai->usefulTimer >= 2 * PI)
			ai->usefulTimer = 0;
		rigidbody->applyCentralForce(cast(_calculateForceDirection(dir, ai->usefulTimer, ai->moveType) * rdbComp->getMass() * 5.f));
		ai->usefulTimer += 1 * delta;
	}
}

void AISystem::registerImGui() {
	
}

glm::vec3 AISystem::_calculateForceDirection(glm::vec3 dir, float time, AIComponent::MovePattern type) {
	glm::vec3 newDir = glm::vec3(0);
	auto right = glm::cross(dir, glm::vec3(0, 1, 0));
	switch (type) {
	case AIComponent::MovePattern::normal:
		newDir = dir;
		break;
	case AIComponent::MovePattern::sinus:
		newDir = glm::normalize(right + dir * sin(time));
		break;
	case AIComponent::MovePattern::circle:
		newDir = glm::vec3(right.x * sin(time), 0, dir.z * cos(time));
		break;
	}

	return newDir;
}