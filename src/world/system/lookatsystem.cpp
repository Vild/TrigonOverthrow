// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "lookatsystem.hpp"

#include "../component/lookatcomponent.hpp"
#include "../component/transformcomponent.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_interpolation.hpp>

#include <glm/glm.hpp>

void LookAtSystem::update(World& world, float delta) {
	for (std::shared_ptr<Entity> entity : world.getEntities()) {
		auto lookat = entity->getComponent<LookAtComponent>();
		if (!lookat || !lookat->target)
			continue;

		auto target = lookat->target->getComponent<TransformComponent>();
		if (!target)
			continue;

		auto transform = entity->getComponent<TransformComponent>();
		if (!transform)
			continue;

		if (lookat->followMode == FollowMode::followByDistance) {
			glm::vec3 dir = glm::normalize(target->getPosition() - transform->getPosition());

			float dist = glm::distance(transform->getPosition(), target->getPosition());

			if (dist > lookat->maxDistance)
				transform->move(dir * delta);
			else if (dist < lookat->minDistance)
				transform->move(-dir * delta);

		} else if (lookat->followMode == FollowMode::followByOffset) {
			transform->setPosition(target->getPosition() + lookat->offsetFromTarget);
		}

		glm::vec3 dir = glm::normalize(target->getPosition() - transform->getPosition());
		transform->setDirection(dir);
	}
}

void LookAtSystem::registerImGui() {}
