#pragma once
#include "system.hpp"
#include "../../gl/shader.hpp"
#include "../src/gl/mesh.hpp"
#include "glm/gtc/matrix_transform.hpp"

class ParticleRenderSystem : public System {
public:
	ParticleRenderSystem();
	virtual void update(World& world, float delta);

private:
	ShaderProgram _program;
};