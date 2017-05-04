#pragma once

#include "component.hpp"

#include "../../lib/glad.h"
#include "glm/glm.hpp"
#include "../../gl/mesh.hpp"

struct ParticleComponent : public Component {
	enum ParticleEffect : int {EXPLOSION = 0, SPEW};
	struct Emitter {
		Emitter(glm::vec3 pos, glm::vec3 dir) { 
			this->pos = pos;
			direction = dir; 
		}
		glm::vec3 pos;
		glm::vec3 direction;
	};
	// Should have a number (1 probably) of emitters to spew out particles from.
	// fix particle render system and a fixed size of particles for each system.

	std::shared_ptr<Emitter> emitter;
	ParticleEffect type;
	int nrOfParticles;
	float particleSize;

	virtual ~ParticleComponent();

	void addEmitter(glm::vec3 pos, glm::vec3 dir, int nrOfParticles) {
		emitter = std::make_shared<Emitter>(pos, dir);
		this->nrOfParticles = nrOfParticles;
		particleSize = 0.4f;
	};

	inline std::string name() final { return "ParticleComponent"; }
	void registerImGui() final;
};
