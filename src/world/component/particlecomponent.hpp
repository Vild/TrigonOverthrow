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
	std::vector<std::shared_ptr<ShaderStorageBuffer>> ssbo;
	float emitterLife;
	bool loaded;

	ParticleComponent() = default;
	ParticleComponent(const ComponentValues& value);
	virtual ~ParticleComponent();
	void addEmitter(glm::vec3 inPos, glm::vec3 dir, ParticleEffect type) {
		emitter = std::make_shared<Emitter>(inPos, dir);
		ssbo.resize(3);
		ssbo[0] = std::make_shared<ShaderStorageBuffer>(1024 * sizeof(glm::vec4));
		ssbo[1] = std::make_shared<ShaderStorageBuffer>(1024 * sizeof(glm::vec4));
		ssbo[2] = std::make_shared<ShaderStorageBuffer>(1024 * sizeof(float));
		loaded = false;
		this->type = type;
		emitterLife = 5;
		std::vector<glm::vec4> particlePositions;
		std::vector<glm::vec4> particleVelocities;
		std::vector<float> particleLives;
		for (int i = 0; i < 1024; i++) {
			particlePositions.push_back(glm::vec4(inPos, 0));
			particleVelocities.push_back(glm::vec4(dir, 0));
			particleLives.push_back(5);
		}
		ssbo[0]->setData(particlePositions);
		ssbo[1]->setData(particleVelocities);
		ssbo[2]->setData(particleLives);
	};

	inline std::string name() final { return "ParticleComponent"; }
	void registerImGui() final;
};
