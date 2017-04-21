// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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

		glm::vec3 eyePos = transformComponent->getPosition();
		glm::vec3 lookDir = transformComponent->getDirection();

		static const glm::vec3 y(0, 1, 0);
		cameraComponent->viewMatrix = glm::lookAt(eyePos, eyePos + lookDir, y);
	}
}

void CameraSystem::registerImGui() {}
