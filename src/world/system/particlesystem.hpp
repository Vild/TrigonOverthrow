#pragma once

#include "system.hpp"

class ParticleSystem : public System {
public:
	virtual void update(World& world, float delta);
};
