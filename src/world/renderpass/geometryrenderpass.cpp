// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "geometryrenderpass.hpp"

#include <glm/gtx/transform.hpp>

#include "../../engine.hpp"
#include "../../lib/glad.h"

#include "../component/transformcomponent.hpp"
#include "../component/modelcomponent.hpp"
#include "../component/cameracomponent.hpp"
#include "../component/instancedsimplemeshcomponent.hpp"

GeometryRenderPass::GeometryRenderPass() {
	auto& engine = Engine::getInstance();

	_gbuffer = std::make_shared<GBuffer>();

	unsigned int width = engine.getWidth(), height = engine.getHeight();
	_gbuffer->bind()
		.attachTexture(Attachment::position, width, height, GL_RGB32F, GL_FLOAT, 3)
		.attachTexture(Attachment::normal, width, height, GL_RGB32F, GL_FLOAT, 3)
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

	_ismShader = std::make_unique<ShaderProgram>();
	(*_ismShader)
		.attach("assets/shaders/ism_geometry.vert", ShaderType::vertex)
		.attach("assets/shaders/ism_geometry.geom", ShaderType::geometry)
		.attach("assets/shaders/ism_geometry.frag", ShaderType::fragment)
		.finalize();

	_ismShader->bind()
		.addUniform("u_view")
		.addUniform("u_projection")
		.addUniform("u_cameraPos")
		.addUniform("u_diffuseSpec")
		.setUniform("u_diffuseSpec", glm::vec4(1));
}

GeometryRenderPass::~GeometryRenderPass() {}

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

	_ismShader->bind();
	_ismShader->setUniform("u_view", cameraComponent->viewMatrix);
	_ismShader->setUniform("u_projection", cameraComponent->projectionMatrix);

	for (Entity* entity : world.getActiveComponents<ModelComponent>()) {
		if (entity->isDead())
			continue;
		ModelComponent* model = entity->getComponent<ModelComponent>();
		TransformComponent* transform = entity->getComponent<TransformComponent>();
		if (transform) {
			_shader->bind();
			model->render(transform->getMatrix());
		}
	}

	for (Entity* entity : world.getActiveComponents<InstancedSimpleMeshComponent>()) {
		if (entity->isDead())
			continue;
		InstancedSimpleMeshComponent* ism = entity->getComponent<InstancedSimpleMeshComponent>();
		_ismShader->bind();
		ism->render();
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
