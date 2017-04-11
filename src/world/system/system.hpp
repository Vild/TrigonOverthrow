#pragma once

#include "../world.hpp"

class System {
public:
	virtual void update(World& world, float delta) = 0;

	virtual void registerImGui() = 0;
	virtual std::string name() = 0;
};
