#pragma once

#include "system.hpp"

class RenderSystem : System {
public:
	virtual void update(World & world, float delta);
};