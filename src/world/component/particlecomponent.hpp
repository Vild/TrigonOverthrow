#pragma once

#include "component.hpp"

#include "../../lib/glad.h"
#include "glm/glm.hpp"
struct ParticleComponent : public Component<ParticleComponent> {

	struct Particle {
		glm::vec3	pos = glm::vec3(0);
		glm::vec3	velocity = glm::vec3(0);
		float		lifeSpan = 0.0f;
	};

	struct Emitter
	{
		Emitter(glm::vec3 dir) { direction = dir; };
		glm::vec3 pos = glm::vec3(0);
		glm::vec3 direction;

	};
	// Should have a number (1 probably) of emitters to spew out particles from.
	// fix particle render system and a fixed size of particles for each system.

	std::shared_ptr<Emitter> emitter;
	std::vector<Particle> particles;
	int _nrOfParticles;
	float particleSize;

	void addEmitter(glm::vec3 dir, int nrOfParticles) { 
		emitter = std::make_shared<Emitter>(dir); 
		_nrOfParticles = nrOfParticles;
		particleSize = 0.4f;
		particles.resize(nrOfParticles);
	};

	virtual std::string name() { return "ParticleComponent"; }

};
