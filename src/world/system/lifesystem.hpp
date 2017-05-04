#pragma once
#include "system.hpp"

class LifeSystem : public System {
public:
	void update(World& world, float delta) final;

	void registerImGui() final;
	inline std::string name() final { return "LifeSystem"; };
};
