#pragma once

#include "../world.hpp"

class System {
public:
	virtual void update(World& world, float delta) = 0;
};