#pragma once

#include "component.hpp"

#include "../../lib/glad.h"
#include "glm/glm.hpp"
#include "../../gl/mesh.hpp"
#include "../../gl/shader.hpp"

struct ParticleComponent : public Component {
	enum ParticleEffect : int {INITIATE = 0, EXPLOSION, SPEW, ORB};
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
	std::vector<std::shared_ptr<ShaderStorageBuffer>> ssbo;
	int nrOfParticles;

	ParticleComponent() = default;
	ParticleComponent(const ComponentValues& value);
	virtual ~ParticleComponent();
	void addEmitter(glm::vec3 inPos, glm::vec3 dir, ParticleEffect type) {
		emitter = std::make_shared<Emitter>(inPos, dir);
		nrOfParticles = 256;
		if (ssbo.size() > 0)
			ssbo.clear();
		ssbo.resize(4);
		ssbo[0] = std::make_shared<ShaderStorageBuffer>(nrOfParticles * sizeof(glm::vec4));
		ssbo[1] = std::make_shared<ShaderStorageBuffer>(nrOfParticles * sizeof(glm::vec4));
		ssbo[2] = std::make_shared<ShaderStorageBuffer>(nrOfParticles * sizeof(float));
		ssbo[3] = std::make_shared<ShaderStorageBuffer>(nrOfParticles * sizeof(glm::vec4));
		this->type = type;
		std::vector<glm::vec4> particlePositions;
		std::vector<glm::vec4> particleVelocities;
		std::vector<glm::vec4> particleColors;
		std::vector<float> particleLives;
		#define frand() (float(rand()) / float(RAND_MAX))

		for (int i = 0; i < nrOfParticles; i++) {
			particlePositions.push_back(glm::vec4(inPos, 0));
			particleVelocities.push_back(glm::vec4(dir, 0));
			particleLives.push_back(frand() * 2);
			if (i <= nrOfParticles / 2)
				particleColors.push_back(glm::vec4(1, 1, 1, 0));
			else
				particleColors.push_back(glm::vec4(0, 0, 0, 0));
		}
		ssbo[0]->setData(particlePositions);
		ssbo[1]->setData(particleVelocities);
		ssbo[2]->setData(particleLives);
		ssbo[3]->setData(particleColors);
	};

	inline std::string name() final { return "ParticleComponent"; }
	void registerImGui() final;
};
