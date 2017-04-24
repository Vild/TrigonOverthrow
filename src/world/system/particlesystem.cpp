// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "particlesystem.hpp"

#include "../component/particlecomponent.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "../../engine.hpp"
#include "../component/guncomponent.hpp"

ParticleSystem::ParticleSystem() {
	_programs.resize(2);
	_programs[0] = std::make_shared<ShaderProgram>();
	_programs[0]->bind().attach(std::make_shared<ShaderUnit>("assets/shaders/particles_init.comp", ShaderType::compute)).finalize();
	_programs[0]->bind().addUniform("delta").addUniform("emitterPos").addUniform("emitterDir").addUniform("entryPos");

	//_programs[1] = std::make_shared<ShaderProgram>();
	//_programs[1]->bind().attach(std::make_shared<ShaderUnit>("assets/shaders/particles_explosion.comp", ShaderType::compute))
	//	.finalize();
	//_programs[1]->bind().addUniform("delta")
	//	.addUniform("swap");
	_textureSize = 32;
	_particleData = std::make_shared<GBuffer>();
	_particleData->bind()
		.attachTexture(Attachment::inPosition, _textureSize, _textureSize, GL_RGBA32F, GL_FLOAT, 4)	// Input pos and life
		.attachTexture(Attachment::inVelocity, _textureSize, _textureSize, GL_RGBA32F, GL_FLOAT, 4); // Input vel
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//#pragma omp parallel for schedule(dynamic, 128)
void ParticleSystem::update(World& world, float delta) {
	// Gotta change how this system works abit. 
	glm::vec3 direction;
	glm::vec3 pos;
	glm::vec3 entryPos;
	for (std::unique_ptr<Entity>& entity : world.getEntities()) {
		auto particleComp = entity->getComponent<ParticleComponent>();
		if (!particleComp)
			continue;

		if (entity->getName() == "Player") {
			auto gunComponent = entity->getComponent<GunComponent>();
			//if (gunComponent->drawShot) {
				auto raygun = std::static_pointer_cast<GunComponent::RayGun>(gunComponent->gun);
				direction = raygun->ray.dir;
				pos = raygun->ray.o;
				entryPos = raygun->ray.t[0];
				_programs[0]
					->bind()
					.setUniform("delta", delta)
					.setUniform("emitterPos", pos)
					.setUniform("emitterDir", direction)
					.setUniform("entryPos", entryPos);
				particleComp->textureSize = _textureSize;
				_particleData->bindImageTexture(0, true);
				_particleData->bindImageTexture(1, true);
				// Barrier is in particlerenderpass.
				glDispatchCompute((GLint)_textureSize, (GLint)_textureSize, 1);
				gunComponent->drawShot = false;
			//}
		}
	}
}

std::shared_ptr<GBuffer> ParticleSystem::getGBuffers() {
	return _particleData;
}

void ParticleSystem::registerImGui() {}
