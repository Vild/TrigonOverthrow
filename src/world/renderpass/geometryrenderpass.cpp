// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "geometryrenderpass.hpp"

#include <glm/gtx/transform.hpp>

#include "../../engine.hpp"
#include "../../lib/glad.h"

#include "../component/transformcomponent.hpp"
#include "../component/floortransformcomponent.hpp"
#include "../component/modelcomponent.hpp"
#include "../component/cameracomponent.hpp"

GeometryRenderPass::GeometryRenderPass() {
	auto& engine = Engine::getInstance();

	_gbuffer = std::make_shared<GBuffer>();

	unsigned int width = engine.getWidth(), height = engine.getHeight();
	_gbuffer->bind()
		.attachTexture(Attachment::position, width, height, GL_RGB32F, GL_FLOAT, 3)
		.attachTexture(Attachment::normal, width, height, GL_RGB, GL_UNSIGNED_BYTE, 3)
		.attachTexture(Attachment::diffuseSpecular, width, height, GL_RGBA, GL_UNSIGNED_BYTE, 4)
		.attachDepthTexture(Attachment::depth, width, height)
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

	_floorShader = std::make_shared<ShaderProgram>();
	_floorShader->attach(std::make_shared<ShaderUnit>("assets/shaders/floorbase.vert", ShaderType::vertex))
		.attach(std::make_shared<ShaderUnit>("assets/shaders/floorbase.geom", ShaderType::geometry))
		.attach(std::make_shared<ShaderUnit>("assets/shaders/floorbase.frag", ShaderType::fragment))
		.finalize();
	_floorShader->bind()
		.addUniform("v")
		.addUniform("p")
		.addUniform("cameraPos")
		.addUniform("diffuseTexture")
		.addUniform("normalTexture")
		.addUniform("setting_doBackFaceCulling")
		.addUniform("setting_defaultSpecular");
	_floorShader->setUniform("diffuseTexture", 0).setUniform("normalTexture", 1).setUniform("setting_defaultSpecular", _setting_base_defaultSpecular);
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

	_floorShader->bind();
	_floorShader->setUniform("v", cameraComponent->viewMatrix);
	_floorShader->setUniform("p", cameraComponent->projectionMatrix);

	_shader->bind();
	_shader->setUniform("v", cameraComponent->viewMatrix);
	_shader->setUniform("p", cameraComponent->projectionMatrix);

	for (std::shared_ptr<Entity>& entity : world.getEntities()) {
		auto model = entity->getComponent<ModelComponent>();
		if (!model)
			continue;

		auto transform = entity->getComponent<TransformComponent>();
		if (transform)
			model->render(transform->matrix);
		else {
			auto ft = entity->getComponent<FloorTransformComponent>();
			if (!ft)
				continue;
			_floorShader->bind();
			glDisable(GL_CULL_FACE);

			model->render(ft->matrices, ft->gridSize * ft->gridSize);
			glEnable(GL_CULL_FACE);
			_shader->bind();
		}
	}
}

void GeometryRenderPass::resize(unsigned int width, unsigned int height) {
	auto& attachmentMap = _gbuffer->getAttachments();
	attachmentMap[Attachment::position]->resize(width, height, GL_RGB32F, GL_RGB, GL_FLOAT);
	attachmentMap[Attachment::normal]->resize(width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
	attachmentMap[Attachment::diffuseSpecular]->resize(width, height, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
	attachmentMap[Attachment::depth]->resize(width, height, GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE);

	_gbuffer->bind();
	glViewport(0, 0, width, height);
}

void GeometryRenderPass::registerImGui() {}
