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
	//_programs.resize(3);
	//_programs[ParticleComponent::ParticleEffect::INITIATE] = std::make_shared<ShaderProgram>();
	//_programs[ParticleComponent::ParticleEffect::INITIATE]->bind().attach(std::make_shared<ShaderUnit>("assets/shaders/particles_init.comp", ShaderType::compute)).finalize();
	//_programs[ParticleComponent::ParticleEffect::INITIATE]->bind().addUniform("emitterCount");
	//
	//_programs[ParticleComponent::ParticleEffect::EXPLOSION] = std::make_shared<ShaderProgram>();
	//_programs[ParticleComponent::ParticleEffect::EXPLOSION]->bind().attach(std::make_shared<ShaderUnit>("assets/shaders/particles_explosion.comp", ShaderType::compute)).finalize();
	//_programs[ParticleComponent::ParticleEffect::EXPLOSION]->bind().addUniform("delta").addUniform("emitterCount");
	//
	//_programs[ParticleComponent::ParticleEffect::SPEW] = std::make_shared<ShaderProgram>();
	//_programs[ParticleComponent::ParticleEffect::SPEW]->bind().attach(std::make_shared<ShaderUnit>("assets/shaders/particles_spew.comp", ShaderType::compute)).finalize();
	//_programs[ParticleComponent::ParticleEffect::SPEW]->bind().addUniform("delta").addUniform("emitterCount");
	//
	//_addEmitterUniforms();
	//
	//_textureSize = 256; // Has support for 32 emitters with 256 particles each.
	//_particleData = std::make_shared<GBuffer>();
	//_particleData->bind()
	//	.attachTexture(Attachment::inPosition, _textureSize, _textureSize, GL_RGBA32F, GL_FLOAT, 4)	// Input pos and life
	//	.attachTexture(Attachment::inVelocity, _textureSize, _textureSize, GL_RGBA32F, GL_FLOAT, 4); // Input vel
	//
	//glClearColor(0, 0, 0, 0);
	//glClear(GL_COLOR_BUFFER_BIT);

	_programs.resize(1);
	_programs[ParticleComponent::ParticleEffect::INITIATE] = std::make_shared<ShaderProgram>();
	_programs[ParticleComponent::ParticleEffect::INITIATE]->bind().attach(std::make_shared<ShaderUnit>("assets/shaders/particles_init.comp", ShaderType::compute)).finalize();
	_programs[ParticleComponent::ParticleEffect::INITIATE]->bind().addUniform("emitterCount").addUniform("delta");
	
	_ssbos.resize(3);
	_ssbos[ParticleAttribute::position] = std::make_shared<ShaderStorageBuffer>(MAX_EMITTER_COUNT * NR_OF_PARTICLES * sizeof(glm::vec3));
	_ssbos[ParticleAttribute::velocity] = std::make_shared<ShaderStorageBuffer>(MAX_EMITTER_COUNT * NR_OF_PARTICLES * sizeof(glm::vec3));
	_ssbos[ParticleAttribute::life] = std::make_shared<ShaderStorageBuffer>(MAX_EMITTER_COUNT * NR_OF_PARTICLES * sizeof(float));
	
	//for (int i = 0; i < 1024; i++) {
	//	glm::vec3 vec;
	//	vec.x = (rand() % 2000) / (500.0);
	//	vec.y = (rand() % 2000) / (500.0);
	//	vec.z = (rand() % 2000) / (500.0);
	//	_computePositions.push_back(vec);
	//}
}

ParticleSystem::~ParticleSystem() {}

//#pragma omp parallel for schedule(dynamic, 128)
void ParticleSystem::update(World& world, float delta) {
	// Gotta change how this system works abit. 
	// Main things to fix: All emitters with the different types should use the correct shader.

	bool noNewData = false;
	std::vector<ParticleComponent::Emitter> emitters;

	// remember to fix initiate particles.
	rmt_ScopedCPUSample(ParticleSystem, RMTSF_None);
	for (std::unique_ptr<Entity>& entity : world.getEntities()) {
		auto particleComp = entity->getComponent<ParticleComponent>();
		if (!particleComp)
			continue;
		if (!particleComp->loaded) {
			particleComp->loaded = true;
			for (int i = 0; i < 1024; i++) {
				_computePositions.push_back(particleComp->particlePositions[i]);
				_computeVelocities.push_back(particleComp->particleVelocities[i]);
				_computeLives.push_back(particleComp->particleLives[i]);
			}
			nrOfEmitters++;
			noNewData = true;
		}
	}
	if (noNewData) {
		_ssbos[ParticleAttribute::position]->setData(_computePositions);
		_ssbos[ParticleAttribute::life]->setData(_computeVelocities);
		_ssbos[ParticleAttribute::velocity]->setData(_computeLives);
	}
	printf("%i\n", nrOfEmitters);
	_programs[ParticleComponent::ParticleEffect::INITIATE]->bind()
		.setUniform("delta", delta);
	_ssbos[ParticleAttribute::position]->bindBase(ParticleAttribute::position);
	_ssbos[ParticleAttribute::velocity]->bindBase(ParticleAttribute::velocity);
	_ssbos[ParticleAttribute::life]->bindBase(ParticleAttribute::life);
	
	glDispatchCompute((GLint)(1024 * nrOfEmitters)/128, 1, 1);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
}

void ParticleSystem::registerImGui() {}
