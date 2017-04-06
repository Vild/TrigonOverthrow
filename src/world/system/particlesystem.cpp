#include "particlesystem.hpp"

#include "../component/particlecomponent.hpp"
#include "glm/gtc/matrix_transform.hpp"

void ParticleSystem::update(World& world, float delta) {
	for (std::shared_ptr<ParticleComponent> comp : ParticleComponent::getActiveComponents()) {
		for (int i = 0; i < comp->_nrOfParticles; i++) {
			comp->particles[i].lifeSpan -= delta * 1.0f;
			if (comp->particles[i].lifeSpan <= 0) {
				comp->particles[i].lifeSpan = 2.0f;
				comp->particles[i].velocity = comp->emitter->direction * delta;
				comp->particles[i].pos = comp->particles[i].velocity;
			}
			comp->particles[i].velocity += comp->particles[i].velocity * delta;
			comp->particles[i].pos += comp->particles[i].velocity;
			comp->particles[i].model += glm::translate(comp->particles[i].model, comp->particles[i].pos);
			comp->matrices[i] = comp->particles[i].model;
		}
	}
}