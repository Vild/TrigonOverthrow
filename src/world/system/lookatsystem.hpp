#pragma once

#include "system.hpp"

class LookAtSystem : public System {
public:
	virtual void update(World& world, float delta);

	virtual void registerImGui();
	inline virtual std::string name() { return "LookAtSystem"; };
};
