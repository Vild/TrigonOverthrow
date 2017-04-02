#pragma once

#include "system.hpp"

class PhysicsSystem : System {
public:
	virtual void update(World & world, float delta);
};