#pragma once

#include "system.hpp"
#include "../src/gl/gbuffer.hpp"
#include "../src/gl/shader.hpp"

class ParticleSystem : public System {
public:
	ParticleSystem();
	virtual void update(World& world, float delta);
private:
	ShaderProgram _program;
	std::shared_ptr<GBuffer> _particleData;
};
