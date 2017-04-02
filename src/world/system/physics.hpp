#pragma once

#include "system.hpp"

class PhysicsSystem : public System {
public:
	virtual void update(World & world, float delta);
};
