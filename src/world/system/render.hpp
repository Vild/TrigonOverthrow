#pragma once

#include "system.hpp"
#include "../../gl/shader.hpp"

class RenderSystem : public System {
public:
	RenderSystem();

	virtual void update(World & world, float delta);

private:
	ShaderProgram _program;
};
