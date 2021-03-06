// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "particlerenderpass.hpp"
#include "../system/particlesystem.hpp"
#include "../component/transformcomponent.hpp"
#include "../component/guncomponent.hpp"
#include "../../engine.hpp"
#include "../component/cameracomponent.hpp"
#include "../component/lookatcomponent.hpp"
#include "../../engine.hpp"
#include <glm/gtx/transform.hpp>
#include <cmath>
#include "glm/glm.hpp"
#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

ParticleRenderPass::ParticleRenderPass() {
	_gbuffer = std::make_shared<GBuffer>(0);
	_shader = std::make_shared<ShaderProgram>();
	_shader->attach(std::make_shared<ShaderUnit>("assets/shaders/particles.vert", ShaderType::vertex))
		.attach(std::make_shared<ShaderUnit>("assets/shaders/particles.frag", ShaderType::fragment))
		.finalize();
	_shader->bind().addUniform("v")
		.addUniform("p");
}

ParticleRenderPass::~ParticleRenderPass() {}

void ParticleRenderPass::render(World& world) {
	// Render particles with instanced drawing.
	auto camera = Engine::getInstance().getCamera();
	if (!camera)
		return;

	auto cameraComponent = camera->getComponent<CameraComponent>();
	if (!cameraComponent)
		return;

	// bind the ssbo as array buffer.
	_shader->bind().setUniform("v", cameraComponent->viewMatrix).setUniform("p", cameraComponent->projectionMatrix);
	for (Entity * entity : world.getActiveComponents<ParticleComponent>()) {
		ParticleComponent * particleComp = entity->getComponent<ParticleComponent>();
		if (particleComp->blend) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		auto ssbos = particleComp->ssbo;
		ssbos[ParticleSystem::ParticleAttribute::position]->bind();
		glEnableVertexAttribArray(12);
		glVertexAttribPointer(12, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
		ssbos[ParticleSystem::ParticleAttribute::velocity]->bind();
		glEnableVertexAttribArray(13);
		glVertexAttribPointer(13, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
		ssbos[ParticleSystem::ParticleAttribute::life]->bind();
		glEnableVertexAttribArray(14);
		glVertexAttribPointer(14, 1, GL_FLOAT, GL_FALSE, sizeof(float), nullptr);
		ssbos[ParticleSystem::ParticleAttribute::color]->bind();
		glEnableVertexAttribArray(15);
		glVertexAttribPointer(15, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		glDrawArrays(GL_POINTS, 0, particleComp->nrOfParticles);
		glDisable(GL_BLEND);
	}
}

void ParticleRenderPass::resize(unsigned int width, unsigned int height) {
	_gbuffer->bind();
	glViewport(0, 0, width, height);
}

void ParticleRenderPass::registerImGui() {}
