#pragma once

#include "system.hpp"

class ImGuiSystem : public System {
public:
	void update(World& world, float delta);
};
