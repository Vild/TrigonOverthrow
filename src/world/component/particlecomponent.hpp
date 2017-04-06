#pragma once

#include "component.hpp"

#include "../../lib/glad.h"
#include "glm/glm.hpp"

struct ParticleComponent : public Component<ParticleComponent> {
	struct Particle {
		glm::vec3	_pos;
		glm::vec3	_speed;
		float		_lifeSpan;
		glm::mat4	_model;
	};

	struct Emitter
	{
		Emitter(glm::vec3 dir) { _direction = dir; };
		glm::vec3 _direction;

	};
	// Should have a number (1 probably) of emitters to spew out particles from.
	// fix particle render system and a fixed size of particles for each system.

	std::shared_ptr<Emitter> _emitter;
	// Should probably be in Emitter if we decide to implement more emitters for one entity.
	int _nrOfParticles;
	Particle _particles[50];

	void addEmitter(glm::vec3 dir) { _emitter = std::make_shared<Emitter>(dir); };
	int getNrOfParticles() { return _nrOfParticles; };
	virtual std::string name() { return "ParticleComponent"; }

};
