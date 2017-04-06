#pragma once
#include "system.hpp"
#include "../../gl/shader.hpp"

class ParticleRenderSystem : public System {
public:
	ParticleRenderSystem();
	virtual void update(World& world, float delta);

private:
	ShaderProgram _program;

};