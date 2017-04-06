#include "particlesystem.hpp"

#include "../component/particlecomponent.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

void ParticleSystem::update(World& world, float delta) {
	for (std::shared_ptr<ParticleComponent> comp : ParticleComponent::getActiveComponents()) {
		for (int i = 0; i < comp->_nrOfParticles; i++) {
			comp->particles[i].lifeSpan -= delta * 1.0f;
			if (comp->particles[i].lifeSpan <= 0) {
				comp->particles[i].lifeSpan = rand() % 5 + 1;
				comp->particles[i].velocity = glm::normalize(glm::vec3(sin(3 * i), cos(i * 0.5f), cos(i * 0.5f) + sin(8 * i) + sin(3 * i)));;
				comp->particles[i].pos = comp->emitter->pos;
			}
			comp->particles[i].velocity += comp->particles[i].velocity * delta;
			comp->particles[i].pos += comp->particles[i].velocity * delta;
			comp->particles[i].model = glm::translate(comp->particles[i].pos) * glm::scale(glm::vec3(0.2));
			comp->matrices[i] = comp->particles[i].model;
			//printf("Lifespan: %f\n", comp->particles[i].lifeSpan);
		}
	}
}