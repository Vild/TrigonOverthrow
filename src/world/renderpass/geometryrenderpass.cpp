#include "geometryrenderpass.hpp"

#include <glm/gtx/transform.hpp>

#include "../../engine.hpp"
#include "../../lib/glad.h"

#include "../component/transformcomponent.hpp"
#include "../component/modelcomponent.hpp"
#include "../component/cameracomponent.hpp"

GeometryRenderPass::GeometryRenderPass() {
	auto& engine = Engine::getInstance();

	_gbuffer = std::make_shared<GBuffer>();

	_gbuffer->bind()
		.attachTexture(Attachment::position, engine.getWidth(), engine.getHeight(), GL_RGB32F, GL_FLOAT, 3)
		.attachTexture(Attachment::normal, engine.getWidth(), engine.getHeight(), GL_RGB, GL_UNSIGNED_BYTE, 3)
		.attachTexture(Attachment::diffuseSpecular, engine.getWidth(), engine.getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, 4)
		.attachDepthTexture(Attachment::depth, engine.getWidth(), engine.getHeight())
		.finalize();

	_shader = std::make_shared<ShaderProgram>();
	_shader->attach(std::make_shared<ShaderUnit>("assets/shaders/base.vert", ShaderType::vertex))
		.attach(std::make_shared<ShaderUnit>("assets/shaders/base.geom", ShaderType::geometry))
		.attach(std::make_shared<ShaderUnit>("assets/shaders/base.frag", ShaderType::fragment))
		.finalize();
	_shader->bind()
		.addUniform("v")
		.addUniform("p")
		.addUniform("cameraPos")
		.addUniform("diffuseTexture")
		.addUniform("normalTexture")
		.addUniform("setting_doBackFaceCulling")
		.addUniform("setting_defaultSpecular");
	_shader->setUniform("diffuseTexture", 0)
		.setUniform("normalTexture", 1)
		.setUniform("setting_doBackFaceCulling", _setting_base_doBackFaceCulling)
		.setUniform("setting_defaultSpecular", _setting_base_defaultSpecular);
}

void GeometryRenderPass::render(World& world) {
	auto camera = Engine::getInstance().getCamera();
	if (!camera)
		return;

	auto cameraComponent = camera->getComponent<CameraComponent>();
	if (!cameraComponent)
		return;

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_shader->bind();

	_shader->setUniform("v", cameraComponent->viewMatrix);
	_shader->setUniform("p", cameraComponent->projectionMatrix);

	for (std::shared_ptr<Entity> entity : world.getEntities()) {
		auto model = entity->getComponent<ModelComponent>();
		if (!model)
			continue;

		auto transform = entity->getComponent<TransformComponent>();
		if (!transform)
			continue;

		model->render(transform->matrix);
	}
}
