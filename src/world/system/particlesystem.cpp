#include "particlesystem.hpp"

#include "../component/particlecomponent.hpp"

void ParticleSystem::update(World& world, float delta) {
	for (std::shared_ptr<ParticleComponent> comp : ParticleComponent::getActiveComponents()) {
		for (int i = 0; i < 50; i++) {
			comp->_particles[i]._lifeSpan -= delta * 1.0f;
			if (comp->_particles[i]._lifeSpan <= 0)
				comp->_particles[i]._lifeSpan = 2.0f;

		}
	}
}