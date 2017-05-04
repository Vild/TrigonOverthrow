#pragma once

#include "system.hpp"

class PhysicsSystem : public System {
public:
	virtual ~PhysicsSystem();

	void update(World& world, float delta) final;

	void registerImGui() final;
	inline std::string name() final { return "PhysicsSystem"; };
};
