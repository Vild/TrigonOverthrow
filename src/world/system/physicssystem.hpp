#pragma once

#include "system.hpp"

class PhysicsSystem : public System {
public:
	virtual ~PhysicsSystem();

	virtual void update(World& world, float delta);

	virtual void registerImGui();
	inline virtual std::string name() { return "PhysicsSystem"; };
};
