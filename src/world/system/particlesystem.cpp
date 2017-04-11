#include "particlesystem.hpp"

#include "../component/particlecomponent.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "../../engine.hpp"

ParticleSystem::ParticleSystem() {
	_programs.resize(2);
	_programs[0] = std::make_shared<ShaderProgram>();
	_programs[0]->bind().attach(std::make_shared<ShaderUnit>("assets/shaders/particles_init.comp", ShaderType::compute))
		.finalize();
	_programs[0]->bind().addUniform("delta").addUniform("emitterPos").addUniform("emitterDir");

	_programs[1] = std::make_shared<ShaderProgram>();
	_programs[1]->bind().attach(std::make_shared<ShaderUnit>("assets/shaders/particles_explosion.comp", ShaderType::compute))
		.finalize();
	_programs[1]->bind().addUniform("delta");

	_particleData = std::make_shared<GBuffer>();
	_particleData->bind().attachTexture(Attachment::inPosition, 32, 32, GL_RGBA32F, GL_FLOAT, 4) // Input pos and life
		.attachTexture(Attachment::inVelocity, 32, 32, GL_RGBA32F, GL_FLOAT, 4)  // Input vel
		.attachTexture(Attachment::outPosition, 32, 32, GL_RGBA32F, GL_FLOAT, 4)  // Output pos and life.
		.attachTexture(Attachment::outVelocity, 32, 32, GL_RGBA32F, GL_FLOAT, 4); // Output vel
	 // TO-DO: Pong pos and vel framebuffers between computations for maximum efficineny.
}

//#pragma omp parallel for schedule(dynamic, 128)
void ParticleSystem::update(World& world, float delta) {
	for (std::shared_ptr<ParticleComponent> comp : ParticleComponent::getActiveComponents()) {
		auto * p = &comp->particles[0];
		int count = comp->_nrOfParticles;
		if (comp->init) {
			_programs[0]->bind().setUniform("delta", delta)
				.setUniform("emitterPos", comp->emitter->pos)
				.setUniform("emitterDir", comp->emitter->direction);
			_particleData->getAttachments()[0]->bind(0);
			_particleData->getAttachments()[1]->bind(1);
			glDispatchCompute(32, 32, 1);
			glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
			comp->init = false;
		}
		else {
			//for (int i = 0; i < count; i++) {
			//	p[i].lifeSpan -= delta * 1.0f;
			//	if (p[i].lifeSpan <= 0) {
			//		p[i].lifeSpan = ((rand()*1.0) / (float)INT_MAX) * 4 + 1;
			//		p[i].velocity = glm::normalize(glm::vec3(sin(3 * i), cos(i * 0.5f), cos(i * 0.5f) + sin(8 * i) + sin(3 * i)));
			//	}
			//	p[i].velocity += p[i].velocity * delta;
			//	p[i].pos += p[i].velocity * delta;
			//
			//}
		}
	}
}

std::shared_ptr<GBuffer> ParticleSystem::getGBuffers() {
	return _particleData;
}