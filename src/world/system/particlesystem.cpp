// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "particlesystem.hpp"

#include "../component/particlecomponent.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "../../engine.hpp"
#include "../component/guncomponent.hpp"
#include "../component/transformcomponent.hpp"

ParticleSystem::ParticleSystem() {
	// Creating shaders for each particle effect. Check particlecomponent.hpp for the enumeration.
	_programs.resize(2);
	_programs[ParticleComponent::ParticleEffect::EXPLOSION] = std::make_shared<ShaderProgram>();
	_programs[ParticleComponent::ParticleEffect::EXPLOSION]->bind().attach(std::make_shared<ShaderUnit>("assets/shaders/particles_explosion.comp", ShaderType::compute)).finalize();
	_programs[ParticleComponent::ParticleEffect::EXPLOSION]->bind().addUniform("delta").addUniform("emitterCount");

	_programs[ParticleComponent::ParticleEffect::SPEW] = std::make_shared<ShaderProgram>();
	_programs[ParticleComponent::ParticleEffect::SPEW]->bind().attach(std::make_shared<ShaderUnit>("assets/shaders/particles_spew.comp", ShaderType::compute)).finalize();
	_programs[ParticleComponent::ParticleEffect::SPEW]->bind().addUniform("delta").addUniform("emitterCount");

	_addEmitterUniforms();

	_textureSize = 256; // Has support for 64 emitters with 1024 particles each.
	_particleData = std::make_shared<GBuffer>();
	_particleData->bind()
		.attachTexture(Attachment::inPosition, _textureSize, _textureSize, GL_RGBA32F, GL_FLOAT, 4)	// Input pos and life
		.attachTexture(Attachment::inVelocity, _textureSize, _textureSize, GL_RGBA32F, GL_FLOAT, 4); // Input vel

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
}

ParticleSystem::~ParticleSystem() {}

//#pragma omp parallel for schedule(dynamic, 128)
void ParticleSystem::update(World& world, float delta) {
	// Gotta change how this system works abit. 
	// Main things to fix: All emitters with the different types should use the correct shader.
	// Will have to do one compute for each effect for all the different emitters/effects whilst them being in the same
	// big texture.

	int emitterCount = 0;
	std::vector<ParticleComponent::Emitter> emitters;
	rmt_ScopedCPUSample(ParticleSystem, RMTSF_None);
	for (std::unique_ptr<Entity>& entity : world.getEntities()) {
		auto particleComp = entity->getComponent<ParticleComponent>();
		if (!particleComp)
			continue;
		emitterCount++;
		emitters.push_back(*particleComp->emitter);
		// Barrier is in particlerenderpass.
		// Alternative 1: Huge texture 256x256 for all 65.536 particles divide them up into 8*8 dispatch into 32x32 local groups.
	}
	// Decide which compute shader to use...
	if (emitterCount > 0) {
		_currEmitterCount = emitterCount;
		_programs[ParticleComponent::ParticleEffect::EXPLOSION]->bind()
			.setUniform("delta", delta)
			.setUniform("emitterCount", emitterCount);
		_setEmitterUniforms(emitters, emitterCount);
		
		_particleData->bindImageTexture(0, true);
		_particleData->bindImageTexture(1, true);
		glDispatchCompute((GLint)_textureSize / (32 * emitterCount), (GLint)_textureSize / (32 * emitterCount), 1);
	}
}

void ParticleSystem::_addEmitterUniforms() {
	for (int i = 0; i < 64; i++) {
		_programs[ParticleComponent::ParticleEffect::EXPLOSION]->bind()
			.addUniform("emitters[" + std::to_string(i) + "].position")
			.addUniform("emitters[" + std::to_string(i) + "].direction");
		_programs[ParticleComponent::ParticleEffect::SPEW]->bind()
			.addUniform("emitters[" + std::to_string(i) + "].position")
			.addUniform("emitters[" + std::to_string(i) + "].direction");
	}
}

void ParticleSystem::_setEmitterUniforms(std::vector<ParticleComponent::Emitter> emitters, const int emitterCount) {
	for (int i = 0; i < emitterCount; i++) {
		_programs[ParticleComponent::ParticleEffect::EXPLOSION]->bind()
			.setUniform("emitters[" + std::to_string(i) + "].position", emitters[i].pos)
			.setUniform("emitters[" + std::to_string(i) + "].direction", emitters[i].direction);
	}
}

std::shared_ptr<GBuffer> ParticleSystem::getGBuffers() {
	return _particleData;
}

void ParticleSystem::registerImGui() {}
