#pragma once

#include "system.hpp"

class LookAtSystem : public System {
public:
	virtual void update(World& world, float delta);
};
