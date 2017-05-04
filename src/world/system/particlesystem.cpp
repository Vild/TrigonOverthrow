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
	_programs[0] = std::make_shared<ShaderProgram>();
	_programs[0]->bind().attach(std::make_shared<ShaderUnit>("assets/shaders/particles_explosion.comp", ShaderType::compute)).finalize();
	_programs[0]->bind().addUniform("delta")
		.addUniform("emitterCount")
		.addUniform("emitters[0].direction")
		.addUniform("emitters[1].direction")
		.addUniform("emitters[2].direction")
		.addUniform("emitters[3].direction")
		.addUniform("emitters[0].position")
		.addUniform("emitters[1].position")
		.addUniform("emitters[2].position")
		.addUniform("emitters[3].position");

	_programs[1] = std::make_shared<ShaderProgram>();
	_programs[1]->bind().attach(std::make_shared<ShaderUnit>("assets/shaders/particles_spew.comp", ShaderType::compute)).finalize();
	_programs[1]->bind().addUniform("delta")
		.addUniform("emitterCount")
		.addUniform("emitters[0].direction")
		.addUniform("emitters[1].direction")
		.addUniform("emitters[2].direction")
		.addUniform("emitters[3].direction")
		.addUniform("emitters[0].position")
		.addUniform("emitters[1].position")
		.addUniform("emitters[2].position")
		.addUniform("emitters[3].position");

	//_programs[1] = std::make_shared<ShaderProgram>();
	//_programs[1]->bind().attach(std::make_shared<ShaderUnit>("assets/shaders/particles_explosion.comp", ShaderType::compute))
	//	.finalize();
	//_programs[1]->bind().addUniform("delta")
	//	.addUniform("swap");
	_textureSize = 256; // Has support for 256 emitters.
	_particleData = std::make_shared<GBuffer>();
	_particleData->bind()
		.attachTexture(Attachment::inPosition, _textureSize, _textureSize, GL_RGBA32F, GL_FLOAT, 4)	// Input pos and life
		.attachTexture(Attachment::inVelocity, _textureSize, _textureSize, GL_RGBA32F, GL_FLOAT, 4); // Input vel

	glClearColor(0, 0, 0, 5);
	glClear(GL_COLOR_BUFFER_BIT);
}

ParticleSystem::~ParticleSystem() {}

//#pragma omp parallel for schedule(dynamic, 128)
void ParticleSystem::update(World& world, float delta) {
	// Gotta change how this system works abit. 
	// Main things to fix: All emitters with the different types should use the correct shader.
	// Will have to do one compute for each effect for all the different emitters/effects whilst them being in the same
	// big texture.


	rmt_ScopedCPUSample(ParticleSystem, RMTSF_None);
	glm::vec3 emittersPos[4] = {
		glm::vec3(0,2,0),
		glm::vec3(2,0,2),
		glm::vec3(4,0,4),
		glm::vec3(6,0,6)
	};

	glm::vec3 emittersDir[4] = {
		glm::vec3(0,1,0),
		glm::vec3(1,1,0),
		glm::vec3(0,0,1),
		glm::vec3(1,1,1)
	};

	for (std::unique_ptr<Entity>& entity : world.getEntities()) {
		auto particleComp = entity->getComponent<ParticleComponent>();
		if (!particleComp)
			continue;

		_programs[particleComp->type]
			->bind()
			.setUniform("delta", delta)
			.setUniform("emitterCount", 4)
			.setUniform("emitters[0].position", emittersPos[0])
			.setUniform("emitters[1].position", emittersPos[1])
			.setUniform("emitters[2].position", emittersPos[2])
			.setUniform("emitters[3].position", emittersPos[3])
			.setUniform("emitters[0].direction", emittersDir[0])
			.setUniform("emitters[1].direction", emittersDir[1])
			.setUniform("emitters[2].direction", emittersDir[2])
			.setUniform("emitters[3].direction", emittersDir[3]);

		particleComp->textureSize = _textureSize;
		_particleData->bindImageTexture(0, true);
		_particleData->bindImageTexture(1, true);
		// Barrier is in particlerenderpass.
		// Alternative 1: Huge texture 512x512 for all particles divide them up into 16x16 dispatch computes for 32x32 local groups.
		// Dispatch compute 16x16 particles
		glDispatchCompute((GLint)_textureSize, (GLint)_textureSize, 1);
	}
}

std::shared_ptr<GBuffer> ParticleSystem::getGBuffers() {
	return _particleData;
}

void ParticleSystem::registerImGui() {}
