#pragma once

#include "system.hpp"
#include "../../gl/gbuffer.hpp"
#include "../../gl/shader.hpp"
#include "../component/particlecomponent.hpp"

class ParticleSystem : public System {
public:
	enum Attachment : GLint { inPosition = 0, inVelocity, outPosition, outVelocity };
	enum ParticleAttribute : int { position = 0, velocity, life};
	struct Positions {
		glm::vec3 positions[];
	};

	struct Velocities {
		glm::vec3 velocities[];
	};

	struct Lives {
		float lives[];
	};

	ParticleSystem();
	virtual ~ParticleSystem();

	void update(World& world, float delta) final;
	void registerImGui() final;
	std::vector<std::shared_ptr<ShaderStorageBuffer>> getSSBO() { return _ssbos; }
	inline std::string name() final { return "ParticleSystem"; }
	std::shared_ptr<GBuffer> getGBuffers();
	int getEmitterCount() const { return nrOfEmitters; }

private:
	std::vector<std::shared_ptr<ShaderProgram>> _programs;
	std::vector<std::shared_ptr<ShaderStorageBuffer>> _ssbos;
	std::vector<glm::vec3> _computePositions;
	std::vector<glm::vec3> _computeVelocities;
	std::vector<float> _computeLives;
	const int MAX_EMITTER_COUNT = 64;
	const int NR_OF_PARTICLES = 1024;
	unsigned int nrOfEmitters;
};
