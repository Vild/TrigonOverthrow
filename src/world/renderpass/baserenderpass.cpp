#include "baserenderpass.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "../../engine.hpp"

#include "../component/transformcomponent.hpp"
#include "../component/modelcomponent.hpp"

BaseRenderPass::BaseRenderPass() {
	_gbuffer = std::make_shared<GBuffer>(0);

	_shader = std::make_shared<ShaderProgram>();
	_shader->attach(std::make_shared<ShaderUnit>("assets/shaders/base.vert", ShaderType::vertex))
		.attach(std::make_shared<ShaderUnit>("assets/shaders/base.frag", ShaderType::fragment))
		.finalize();
	_shader->bind().addUniform("m").addUniform("v").addUniform("p");
}

void BaseRenderPass::render(World& world) {
	auto camera = Engine::getInstance().getCamera();
	if (!camera)
		return;

	auto cameraTransform = camera->getComponent<TransformComponent>();
	if (!cameraTransform)
		return;

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_shader->bind();

	float _fov = 80.0f; // XXX: Extract to CameraEntity

	// Everything needed for the view matrix is in the rotation matrix
	_shader->setUniform("v", cameraTransform->rotation);
	_shader->setUniform("p", glm::perspective(glm::radians(_fov), (float)1280 / (float)720, 0.1f, 60.0f));

	for (std::shared_ptr<Entity> entity : world.getEntities()) {
		auto model = entity->getComponent<ModelComponent>();
		if (!model)
			continue;

		auto transform = entity->getComponent<TransformComponent>();
		if (!transform)
			continue;

		_shader->setUniform("m", glm::translate(transform->position) * glm::scale(transform->scale) * transform->rotation);

		model->render();
	}
}
