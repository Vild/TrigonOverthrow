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
	std::shared_ptr<Mesh> point;
	ParticleEffect type;
	int _nrOfParticles;
	float particleSize;
	int textureSize;

	virtual ~ParticleComponent();

	void addEmitter(glm::vec3 pos, glm::vec3 dir, int nrOfParticles) {
		emitter = std::make_shared<Emitter>(pos, dir);
		_nrOfParticles = nrOfParticles;
		particleSize = 0.4f;
		std::vector<Vertex> vertices = {Vertex{glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec3(1, 1, 1), glm::vec2(0, 0), glm::vec3(0, 0, 1)}};
		std::vector<GLuint> indices = {0};
		point = std::make_shared<Mesh>(vertices, indices);
	};

	virtual std::string name() { return "ParticleComponent"; }
	virtual void registerImGui(){};
};
