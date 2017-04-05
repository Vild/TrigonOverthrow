#pragma once

#include "system.hpp"

class InputSystem : public System {
public:
	void update(World& world, float delta);
};