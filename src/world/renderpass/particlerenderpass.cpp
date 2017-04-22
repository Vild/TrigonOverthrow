// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "particlerenderpass.hpp"
#include "../component/particlecomponent.hpp"
#include "../component/transformcomponent.hpp"
#include "../../engine.hpp"
#include "../component/cameracomponent.hpp"
#include "../component/lookatcomponent.hpp"
#include <glm/gtx/transform.hpp>
#include <cmath>
#include "glm/glm.hpp"

ParticleRenderPass::ParticleRenderPass() {
	_gbuffer = std::make_shared<GBuffer>(0);
	_shader = std::make_shared<ShaderProgram>();
	_shader->attach(std::make_shared<ShaderUnit>("assets/shaders/particles.vert", ShaderType::vertex))
		.attach(std::make_shared<ShaderUnit>("assets/shaders/particles.frag", ShaderType::fragment))
		.finalize();
	_shader->bind().addUniform("v").addUniform("p").addUniform("particlePos").addUniform("particleVel").addUniform("textureSize");
	_shader->setUniform("particlePos", (GLint)InputAttachment::position).setUniform("particleVel", (GLint)InputAttachment::velocity);
}

void ParticleRenderPass::render(World& world) {
	// Render particles with instanced drawing.
	auto camera = Engine::getInstance().getCamera();
	if (!camera)
		return;

	auto cameraComponent = camera->getComponent<CameraComponent>();
	if (!cameraComponent)
		return;

	_shader->bind().setUniform("v", cameraComponent->viewMatrix).setUniform("p", cameraComponent->projectionMatrix);
	for (std::unique_ptr<Entity>& entity : world.getEntities()) {
		auto particle = entity->getComponent<ParticleComponent>();
		if (!particle)
			continue;
		_shader->setUniform("billboardSize", particle->particleSize);
		_shader->setUniform("textureSize", particle->textureSize);
		// wait for reading/writing before rendering.

		glMemoryBarrier(GL_ALL_BARRIER_BITS);
		particle->point->render(particle->_nrOfParticles, GL_POINTS);
	}
}

void ParticleRenderPass::resize(unsigned int width, unsigned int height) {
	_gbuffer->bind();
	glViewport(0, 0, width, height);
}

void ParticleRenderPass::registerImGui() {}
