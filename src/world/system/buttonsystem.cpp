#include "buttonsystem.hpp"

#include "../component/transformcomponent.hpp"
#include "../component/buttoncomponent.hpp"
#include "../component/cameracomponent.hpp"

void ButtonSystem::update(World& world, float delta) {
	Engine& engine = Engine::getInstance();
	auto camera = engine.getCamera();
	if (!camera)
		return;

	auto cameraComponent = camera->getComponent<CameraComponent>();
	if (!cameraComponent)
		return;

	const glm::ivec2 xy = engine.getHIDInput().getXY();
	unsigned int w = engine.getWidth(), h = engine.getHeight();

	Entity* hit = nullptr;
	float zHit = std::numeric_limits<float>::infinity();

	const glm::vec2 rayNDC = glm::vec2{(2.0 * xy.x) / w - 1.0, 1.0 - (2.0 * xy.y) / h};
	const glm::vec4 rayClip = glm::vec4(rayNDC, -1, 1);
	const glm::vec4 rayEye = glm::vec4(inverse(cameraComponent->projectionMatrix) * rayClip).xy, -1, 0);
	const glm::vec3 rawWorld = glm::normalize((inverse(cameraComponent->viewMatrix) * rayEye).xyz);

	for (std::unique_ptr<Entity>& entity : world.getEntities()) {
		auto button = entity->getComponent<ButtonComponent>();
		if (!button)
			return;

		auto transform = entity->getComponent<TransformComponent>();
		if (!transform)
			return;

	}
}

void ButtonSystem::registerImGui() {}
