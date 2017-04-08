#include "camerasystem.hpp"
#include "../component/transformcomponent.hpp"
#include "../component/cameracomponent.hpp"
#include <glm/gtx/transform.hpp>

void CameraSystem::update(World& world, float delta) {
	for (std::shared_ptr<Entity> entity : world.getEntities()) {
		auto cameraComponent = entity->getComponent<CameraComponent>();
		if (!cameraComponent)
			continue;

		auto transformComponent = entity->getComponent<TransformComponent>();
		if (!transformComponent)
			continue;

		// glm::vec3 lookDir = transformComponent->getDirection();

		static const glm::vec3 y(0, 1, 0);
		cameraComponent->viewMatrix = glm::lookAt(transformComponent->position, transformComponent->position + transformComponent->getDirection(), y);
	}
}
