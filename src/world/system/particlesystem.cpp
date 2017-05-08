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
	_programs.resize(1);
	_programs[ParticleComponent::ParticleEffect::INITIATE] = std::make_shared<ShaderProgram>();
	_programs[ParticleComponent::ParticleEffect::INITIATE]->bind().attach(std::make_shared<ShaderUnit>("assets/shaders/particles_init.comp", ShaderType::compute)).finalize();
	_programs[ParticleComponent::ParticleEffect::INITIATE]->bind().addUniform("delta");

	//_programs[ParticleComponent::ParticleEffect::EXPLOSION] = std::make_shared<ShaderProgram>();
	//_programs[ParticleComponent::ParticleEffect::EXPLOSION]->bind().attach(std::make_shared<ShaderUnit>("assets/shaders/particles_explosion.comp", ShaderType::compute)).finalize();
	//_programs[ParticleComponent::ParticleEffect::EXPLOSION]->bind().addUniform("delta");
	//
	//_programs[ParticleComponent::ParticleEffect::SPEW] = std::make_shared<ShaderProgram>();
	//_programs[ParticleComponent::ParticleEffect::SPEW]->bind().attach(std::make_shared<ShaderUnit>("assets/shaders/particles_spew.comp", ShaderType::compute)).finalize();
	//_programs[ParticleComponent::ParticleEffect::SPEW]->bind().addUniform("delta");

	_ssbos.resize(3);
	_ssbos[ParticleAttribute::position] = std::make_shared<ShaderStorageBuffer>(MAX_EMITTER_COUNT * NR_OF_PARTICLES * sizeof(glm::vec4));
	_ssbos[ParticleAttribute::velocity] = std::make_shared<ShaderStorageBuffer>(MAX_EMITTER_COUNT * NR_OF_PARTICLES * sizeof(glm::vec4));
	_ssbos[ParticleAttribute::life] = std::make_shared<ShaderStorageBuffer>(MAX_EMITTER_COUNT * NR_OF_PARTICLES * sizeof(float));
}

ParticleSystem::~ParticleSystem() {}

//#pragma omp parallel for schedule(dynamic, 128)
void ParticleSystem::update(World& world, float delta) {
	// Gotta change how this system works abit.
	// Main things to fix: All emitters with the different types should use the correct shader.

	bool newData = false;
	int innerCounter = 0;
	int emitters[64];
	int newEmittersCount = 0;
	// remember to fix initiate particles.
	rmt_ScopedCPUSample(ParticleSystem, RMTSF_None);
	for (std::unique_ptr<Entity>& entity : world.getEntities()) {
		auto particleComp = entity->getComponent<ParticleComponent>();
		if (!particleComp)
			continue;

		if (particleComp->emitterLife <= 0) {
			// Gotta fix this, temporarily makes them respawn.
			//entity->removeComponent<ParticleComponent>();
			//entity->makeDead();
			//_removeEmitter(innerCounter);
			//printf("Emitter: %i removed...\n", innerCounter);
			//continue;
			particleComp->emitterLife = 5.0f;
			particleComp->loaded = false;
		}

		if (!particleComp->loaded) {
			particleComp->loaded = true;
			for (int i = 0; i < 1024; i++) {
				_computePositions.push_back(glm::vec4(particleComp->particlePositions[i], 0));
				_computeVelocities.push_back(glm::vec4(particleComp->particleVelocities[i], 0));
				_computeLives.push_back(particleComp->particleLives[i]);
			}
			nrOfEmitters++;
			emitters[newEmittersCount] = innerCounter;
			newEmittersCount++;
			newData = true;
		}
		particleComp->emitterLife -= 1 * delta;
		innerCounter++;
	}
	if (newData)
		_addNewData(emitters, newEmittersCount);

	if (nrOfEmitters > 0) {
		_programs[ParticleComponent::ParticleEffect::INITIATE]->bind()
			.setUniform("delta", delta);
		_ssbos[ParticleAttribute::position]->bindBase(ParticleAttribute::position);
		_ssbos[ParticleAttribute::velocity]->bindBase(ParticleAttribute::velocity);
		_ssbos[ParticleAttribute::life]->bindBase(ParticleAttribute::life);
		glDispatchCompute((GLint)(1024 * nrOfEmitters)/128, 1, 1);
	}
}

void ParticleSystem::_addNewData(int emitters[64], int tempNrOfEmitters) {
	for (int i = 0; i < tempNrOfEmitters; i++) {
		_ssbos[ParticleAttribute::position]->setSpecificSubData(_computePositions,
			NR_OF_PARTICLES * emitters[i] * sizeof(glm::vec4), NR_OF_PARTICLES, emitters[i]);
		_ssbos[ParticleAttribute::velocity]->setSpecificSubData(_computeVelocities,
			NR_OF_PARTICLES * emitters[i] * sizeof(glm::vec4), NR_OF_PARTICLES, emitters[i]);
		_ssbos[ParticleAttribute::life]->setSpecificSubData(_computeLives,
			NR_OF_PARTICLES * emitters[i] * sizeof(float), NR_OF_PARTICLES, emitters[i]);
	}
}

void ParticleSystem::_removeEmitter(const int counter) {
	for (int i = 0; i < 1024; i++) {
		_computePositions[counter * NR_OF_PARTICLES + i] = glm::vec4(0);
		_computeVelocities[counter * NR_OF_PARTICLES + i] = glm::vec4(0);
		_computeLives[counter * NR_OF_PARTICLES + i] = -10;
	}
	_computePositions.erase(remove(_computePositions.begin(), _computePositions.end(), glm::vec4(0)),
		_computePositions.end());
	_computeVelocities.erase(remove(_computeVelocities.begin(), _computeVelocities.end(), glm::vec4(0)),
		_computeVelocities.end());
	_computeLives.erase(remove(_computeLives.begin(), _computeLives.end(), -10),
		_computeLives.end());
	nrOfEmitters -= 1;
}

void ParticleSystem::registerImGui() {}
