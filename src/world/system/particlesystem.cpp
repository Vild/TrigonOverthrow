// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "particlesystem.hpp"

#include "../component/particlecomponent.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "../../engine.hpp"

ParticleSystem::ParticleSystem() {
	_programs.resize(2);
	_programs[0] = std::make_shared<ShaderProgram>();
	_programs[0]->bind().attach(std::make_shared<ShaderUnit>("assets/shaders/particles_init.comp", ShaderType::compute)).finalize();
	_programs[0]->bind().addUniform("delta").addUniform("emitterPos").addUniform("emitterDir");

	//_programs[1] = std::make_shared<ShaderProgram>();
	//_programs[1]->bind().attach(std::make_shared<ShaderUnit>("assets/shaders/particles_explosion.comp", ShaderType::compute))
	//	.finalize();
	//_programs[1]->bind().addUniform("delta")
	//	.addUniform("swap");
	textureSize = 128;
	_particleData = std::make_shared<GBuffer>();
	_particleData->bind()
		.attachTexture(Attachment::inPosition, textureSize, textureSize, GL_RGBA32F, GL_FLOAT, 4)	// Input pos and life
		.attachTexture(Attachment::inVelocity, textureSize, textureSize, GL_RGBA32F, GL_FLOAT, 4); // Input vel
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//#pragma omp parallel for schedule(dynamic, 128)
void ParticleSystem::update(World& world, float delta) {
	for (std::shared_ptr<ParticleComponent> comp : ParticleComponent::getActiveComponents()) {
		_programs[0]
			->bind()
			.setUniform("delta", delta)
			.setUniform("emitterPos", comp->emitter->pos)
			.setUniform("emitterDir", comp->emitter->direction)
			.setUniform("init", comp->init);
		comp->init = false;
		comp->textureSize = textureSize;
		_particleData->bindImageTexture(0, true);
		_particleData->bindImageTexture(1, true);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Barrier is in particlerenderpass.
		glDispatchCompute((GLint)textureSize, (GLint)textureSize, 1);
	}
}

std::shared_ptr<GBuffer> ParticleSystem::getGBuffers() {
	return _particleData;
}

void ParticleSystem::registerImGui() {}
