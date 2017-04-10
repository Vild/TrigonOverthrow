#include "particlerenderpass.hpp"
#include "../component/particlecomponent.hpp"
#include "../component/transformcomponent.hpp"
#include "../src/engine.hpp"
#include "../component/cameracomponent.hpp"
#include "../component/lookatcomponent.hpp"
#include <glm/gtx/transform.hpp>
#include <cmath>

ParticleRenderPass::ParticleRenderPass() {
	_shader = std::make_shared<ShaderProgram>();
	_shader->attach(std::make_shared<ShaderUnit>("assets/shaders/particles.vert", ShaderType::vertex))
		.attach(std::make_shared<ShaderUnit>("assets/shaders/particles.frag", ShaderType::fragment))
		.finalize();
	_shader->bind().addUniform("cameraRight_wPos")
		.addUniform("cameraUp_wPos")
		.addUniform("billboardSize")
		.addUniform("cameraPos")
		.addUniform("v")
		.addUniform("p");
}

void ParticleRenderPass::render(World& world) {
	// Render particles with instanced drawing.
	auto camera = Engine::getInstance().getCamera();
	if (!camera)
		return;

	auto cameraComponent = camera->getComponent<CameraComponent>();
	if (!cameraComponent)
		return;

	auto lookAtComponent = camera->getComponent<LookAtComponent>();
	if (!lookAtComponent)
		return;

	float _fov = 80.0f; // XXX: Extract to CameraEntity

	_shader->bind().setUniform("cameraRight_wPos", glm::vec3(-1, 0, 0))
		.setUniform("cameraUp_wPos", glm::vec3(0, 0.894, 0.447))
		.setUniform("cameraPos", lookAtComponent->offsetFromTarget)
		.setUniform("v", cameraComponent->viewMatrix)
		.setUniform("p", cameraComponent->projectionMatrix);

	for (std::shared_ptr<Entity> entity : world.getEntities()) {
		auto particle = entity->getComponent<ParticleComponent>();
		if (!particle)
			continue;
		_shader->setUniform("billboardSize", particle->particleSize);
		particle->_quad->uploadBufferArray("m", particle->matrices);
		particle->_quad->render(particle->_nrOfParticles);
	}

}

void ParticleRenderPass::resize(unsigned int width, unsigned int height) {
	auto& attachmentMap = _gbuffer->getAttachments();
	attachmentMap[Attachment::position]->resize(width, height, GL_RGB32F, GL_RGB, GL_FLOAT);
	attachmentMap[Attachment::normal]->resize(width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
	attachmentMap[Attachment::diffuseSpecular]->resize(width, height, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
	attachmentMap[Attachment::depth]->resize(width, height, GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE);
	
	_gbuffer->bind();
	glViewport(0, 0, width, height);
}