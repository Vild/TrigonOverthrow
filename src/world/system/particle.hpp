#pragma once

#include "system.hpp"

class ParticleSystem : System {
public:
	virtual void update(World & world, float delta);
};