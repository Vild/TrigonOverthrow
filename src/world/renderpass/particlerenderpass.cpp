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

ParticleRenderPass::ParticleRenderPass() {
	_gbuffer = std::make_shared<GBuffer>(0);
	_shader = std::make_shared<ShaderProgram>();
	_shader->attach(std::make_shared<ShaderUnit>("assets/shaders/particles.vert", ShaderType::vertex))
		.attach(std::make_shared<ShaderUnit>("assets/shaders/particles.frag", ShaderType::fragment))
		.finalize();
	_shader->bind().addUniform("v")
		.addUniform("p")
		.addUniform("particlePos")
		.addUniform("particleVel")
		.addUniform("textureSize")
		.addUniform("emitterCount");
	_shader->setUniform("particlePos", (GLint)InputAttachment::position).setUniform("particleVel", (GLint)InputAttachment::velocity);

	std::vector<Vertex> vertices = { Vertex{ glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec3(1, 1, 1), glm::vec2(0, 0), glm::vec3(0, 0, 1) } };
	std::vector<GLuint> indices = { 0 };
	_point = std::make_shared<Mesh>(vertices, indices);
}

ParticleRenderPass::~ParticleRenderPass() {}

void ParticleRenderPass::render(World& world) {
	rmt_ScopedCPUSample(ParticleRenderPass, RMTSF_None);
	rmt_ScopedOpenGLSample(ParticleRenderPass);
	// Render particles with instanced drawing.
	auto camera = Engine::getInstance().getCamera();
	if (!camera) 
		return;

	auto cameraComponent = camera->getComponent<CameraComponent>();
	if (!cameraComponent)
		return;

	_shader->bind().setUniform("v", cameraComponent->viewMatrix).setUniform("p", cameraComponent->projectionMatrix);
	auto emitterCount = Engine::getInstance().getSystem<ParticleSystem>()->getCurrEmitterCount();
	if (emitterCount > 0) {
		_shader->setUniform("billboardSize", 2)
			.setUniform("textureSize", Engine::getInstance().getSystem<ParticleSystem>()->getTextureSize())
			.setUniform("emitterCount", emitterCount);
		// wait for reading/writing before rendering.
		glMemoryBarrier(GL_ALL_BARRIER_BITS);
		// Should only be in one spot if it's correct.
		_point->render(NR_OF_PARTICLES * emitterCount, GL_POINTS);
	}
	//for (std::unique_ptr<Entity>& entity : world.getEntities()) {
	//	auto particle = entity->getComponent<ParticleComponent>();
	//	if (!particle)
	//		continue;
	//	_shader->setUniform("billboardSize", particle->particleSize);
	//	_shader->setUniform("textureSize", Engine::getInstance().getSystem<ParticleSystem>()->getTextureSize());
	//	// wait for reading/writing before rendering.
	//
	//	glMemoryBarrier(GL_ALL_BARRIER_BITS);
	//	particle->point->render(particle->nrOfParticles, GL_POINTS);
	//}
}

void ParticleRenderPass::resize(unsigned int width, unsigned int height) {
	_gbuffer->bind();
	glViewport(0, 0, width, height);
}

void ParticleRenderPass::registerImGui() {}
