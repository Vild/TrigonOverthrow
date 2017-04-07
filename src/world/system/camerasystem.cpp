#include "camerasystem.hpp"
#include "../component/transformcomponent.hpp"
#include "../component/cameracomponent.hpp"

void CameraSystem::update(World & world, float delta)
{
	for (std::shared_ptr<Entity> entity : world.getEntities())
	{
		auto cameraComponent = entity->getComponent<CameraComponent>();
		if (!cameraComponent)
			continue;

		auto transformComponent = entity->getComponent<TransformComponent>();
		if (!transformComponent)
			continue;

		glm::vec3 lookDir = transformComponent->getDirection();

		cameraComponent->lookAt(transformComponent->position, transformComponent->position + lookDir);
	}
}
