#pragma once

#include "system.hpp"
#include "../src/gl/gbuffer.hpp"
#include "../src/gl/shader.hpp"

class ParticleSystem : public System {
public:
	enum Attachment : GLint { inPosition = 0, inVelocity, outPosition, outVelocity };

	ParticleSystem();
	virtual void update(World& world, float delta);
	std::shared_ptr<GBuffer> getGBuffers();
private:
	std::vector<std::shared_ptr<ShaderProgram>> _programs;
	std::shared_ptr<GBuffer> _particleData;
};
