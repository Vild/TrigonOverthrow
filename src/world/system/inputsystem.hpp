#pragma once

#include "system.hpp"

class InputSystem : public System {
public:
	virtual ~InputSystem();

	void update(World& world, float delta) final;

	void registerImGui() final;
	inline std::string name() final { return "InputSystem"; };
};
