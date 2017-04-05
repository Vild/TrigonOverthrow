#include "rendersystem.hpp"

#include "../../engine.hpp"
#include "../component/modelcomponent.hpp"
#include "../component/transformcomponent.hpp"

#include <glm/gtx/transform.hpp>

#include <cmath>

RenderSystem::RenderSystem() {
	_program.attach(std::make_shared<ShaderUnit>("assets/shaders/base.vert", ShaderType::vertex))
		.attach(std::make_shared<ShaderUnit>("assets/shaders/base.frag", ShaderType::fragment))
		.finalize();
	_program.bind().addUniform("m").addUniform("v").addUniform("p");
}

void RenderSystem::update(World& world, float delta) {
	auto camera = Engine::getInstance().getCamera();
	if (!camera)
		return;

	auto cameraTransform = camera->getComponent<TransformComponent>();
	if (!cameraTransform)
		return;

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_program.bind();

	float _fov = 80.0f; // XXX: Extract to CameraEntity

	// Everything needed for the view matrix is in the rotation matrix
	_program.setUniform("v", cameraTransform->rotation);
	_program.setUniform("p", glm::perspective(glm::radians(_fov), (float)1280 / (float)720, 0.1f, 60.0f));

	for (std::shared_ptr<Entity> entity : world.getEntities()) {
		auto model = entity->getComponent<ModelComponent>();
		if (!model)
			continue;

		auto transform = entity->getComponent<TransformComponent>();
		if (!transform)
			continue;

		_program.setUniform("m", glm::translate(transform->position) * glm::scale(transform->scale) * transform->rotation);

		model->render();
	}
}
