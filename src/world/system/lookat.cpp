#include "lookat.hpp"

#include "../component/lookat.hpp"
#include "../component/transform.hpp"

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

		glm::vec3 dir = glm::normalize(target->position - transform->position);

		float dist = glm::distance(transform->position, target->position);
		if (dist > lookat->maxDistance)
			transform->position += dir * delta;
		else if (dist < lookat->minDistance)
			transform->position -= dir * delta;

		// transform->rotation = glm::lookAt(transform->position, transform->position + dir, glm::vec3(0, 1, 0));
	}
}
