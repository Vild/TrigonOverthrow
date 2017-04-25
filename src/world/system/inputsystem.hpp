#pragma once

#include "system.hpp"

class InputSystem : public System {
public:
	virtual ~InputSystem();

	void update(World& world, float delta);

	virtual void registerImGui();
	inline virtual std::string name() { return "InputSystem"; };
};
