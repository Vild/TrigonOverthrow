#include "render.hpp"

#include "../../engine.hpp"
#include "../component/model.hpp"
#include "../component/transform.hpp"

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

	// XXX: Extract to CameraEntity
	float _fov = 80.0f;

	/*float _yaw = 0; // +Z (at 0, 0, 0)
	float _pitch = 0.0f;
	glm::vec3 _position = //glm::vec3(0, 0, -2);

	glm::vec3 forward(cos(_pitch) * sin(_yaw), sin(_pitch), cos(_pitch) * cos(_yaw));
	glm::vec3 right(sin(_yaw - M_PI / 2.0f), 0, cos(_yaw - M_PI / 2.0f));
	glm::vec3 up = glm::cross(right, forward);*/

	//_program.setUniform("v", glm::lookAt(_position, _position + forward, up));
	_program.setUniform("v", glm::translate(cameraTransform->position) * glm::scale(cameraTransform->scale) * cameraTransform->rotation);
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
