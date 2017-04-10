#include "particlesystem.hpp"

#include "../component/particlecomponent.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "../../engine.hpp"

ParticleSystem::ParticleSystem() {
	//_programs.resize(2);
	//_programs[0]->bind().attach(std::make_shared<ShaderUnit>("assets/shaders/particles_init.comp", ShaderType::compute))
	//	.finalize();
	//_programs[1]->bind().attach(std::make_shared<ShaderUnit>("assets/shaders/particles_explosion.comp", ShaderType::compute))
	//	.finalize();

	// TO-DO: Pong pos and vel framebuffers between computations for maximum efficineny.





	//_particleData = std::make_shared<GBuffer>();
	//_particleData->bind().attachTexture(0, 512, 512, GL_RGBA32F, GL_FLOAT, 3)
	//	.attachTexture(1, 512, 512, GL_RGBA32F, GL_FLOAT, 4)
	//	.attachTexture(2, 512, 512, GL_RGBA32F, GL_FLOAT, 4)  // Output pos and life.
	//	.attachTexture(3, 512, 512, GL_RGBA32F, GL_FLOAT, 4); // Output vel
	//_program.bind().addUniform("delta");

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ParticleSystem::update(World& world, float delta) {
	for (std::shared_ptr<ParticleComponent> comp : ParticleComponent::getActiveComponents()) {
		auto * p = &comp->particles[0];
		auto * m = &comp->matrices[0];
		int count = comp->_nrOfParticles;
		glm::vec3 spawn = comp->emitter->pos;

		//#pragma omp parallel for schedule(dynamic, 128)
		for (int i = 0; i < count; i++) {
			p[i].lifeSpan -= delta * 1.0f;
			if (p[i].lifeSpan <= 0) {
				p[i].lifeSpan = ((rand()*1.0) / (float)INT_MAX) * 4 + 1;
				p[i].velocity = glm::normalize(glm::vec3(sin(3 * i), cos(i * 0.5f), cos(i * 0.5f) + sin(8 * i) + sin(3 * i)));;
				p[i].pos = spawn;
			}
			p[i].velocity += p[i].velocity * delta;
			p[i].pos += p[i].velocity * delta;
			p[i].model = glm::translate(p[i].pos) * glm::scale(glm::vec3(0.2));
			m[i] = p[i].model;
		}
	}
}