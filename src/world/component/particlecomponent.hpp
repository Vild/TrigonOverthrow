#pragma once

#include "component.hpp"

#include "../../lib/glad.h"
#include "glm/glm.hpp"
#include "../../gl/mesh.hpp"
#include "../../gl/shader.hpp"

struct ParticleComponent : public Component {
	enum ParticleEffect : int {INITIATE = 0, EXPLOSION, SPEW};
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
	glm::vec3 particlePositions[1024];
	glm::vec3 particleVelocities[1024];
	float particleLives[1024];
	bool loaded;
	virtual ~ParticleComponent();

	void addEmitter(glm::vec3 inPos, glm::vec3 dir, int nrOfParticles) {
		emitter = std::make_shared<Emitter>(inPos, dir);
		this->nrOfParticles = nrOfParticles;
		particleSize = 0.4f;
		loaded = false;
		for (int i = 0; i < 1024; i++) {
			glm::vec3 pos;
			float life;
			//pos.x = ((rand() % 2000) / (500.0));
			//pos.y = ((rand() % 2000) / (500.0));
			//pos.z = ((rand() % 2000) / (500.0));
			particlePositions[i] = pos + inPos;
			particleVelocities[i] = dir;
			life = (rand() % 10 + 1);
		}
	};

	inline std::string name() final { return "ParticleComponent"; }
	void registerImGui() final;
};
