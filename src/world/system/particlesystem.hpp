#pragma once

#include "system.hpp"
#include "../../gl/gbuffer.hpp"
#include "../../gl/shader.hpp"
#include "../component/particlecomponent.hpp"

class ParticleSystem : public System {
public:
	enum Attachment : GLint { inPosition = 0, inVelocity, outPosition, outVelocity };
	enum ParticleAttribute : int { position = 0, velocity, life, color};

	ParticleSystem();
	virtual ~ParticleSystem();

	void update(World& world, float delta) final;
	void registerImGui() final;
	inline std::string name() final { return "ParticleSystem"; }

private:
	std::vector<std::shared_ptr<ShaderProgram>> _programs;
	const int MAX_EMITTER_COUNT = 64;
	const int NR_OF_PARTICLES = 256;
};
