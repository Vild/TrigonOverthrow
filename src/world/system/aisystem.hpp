#pragma once
#include "system.hpp"

class AISystem : public System {

	void update(World& world, float delta) final;
	void registerImGui() final;
	inline std::string name() final { return "AISystem"; }
};