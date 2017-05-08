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
	glm::vec3 particlePositions[1024];
	glm::vec3 particleVelocities[1024];
	float particleLives[1024];
	float emitterLife;
	bool loaded;

	ParticleComponent() = default;
	ParticleComponent(const ComponentValues& value);
	virtual ~ParticleComponent();
	void addEmitter(glm::vec3 inPos, glm::vec3 dir, ParticleEffect type) {
		emitter = std::make_shared<Emitter>(inPos, dir);
		loaded = false;
		this->type = type;
		emitterLife = 5;
		switch (type)
		{
		case ParticleComponent::INITIATE: {
			for (int i = 0; i < 1024; i++) {
				glm::vec3 pos;
				pos.x = ((rand() % 2000) / (500.0));
				pos.y = ((rand() % 2000) / (500.0));
				pos.z = ((rand() % 2000) / (500.0));
				particlePositions[i] = pos + inPos;
				particleVelocities[i] = dir;
				particleLives[i] = 5;
			}
			break;
		}
		case ParticleComponent::EXPLOSION: {
			for (int i = 0; i < 1024; i++) {
				glm::vec3 vel;
				particlePositions[i] = inPos;
				particleVelocities[i] = glm::normalize(glm::vec3(sin(3 * i), abs(cos(i * 0.5f)), cos(i * 0.5f) - sin(8 * i) + sin(3 * i)));
				particleLives[i] = 5;
			}
			break;
		}
		case ParticleComponent::SPEW: {

			break;
		}
		default:
			break;
		}
	};

	inline std::string name() final { return "ParticleComponent"; }
	void registerImGui() final;
};
