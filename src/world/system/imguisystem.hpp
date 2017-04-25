#pragma once

#include "system.hpp"

class ImGuiSystem : public System {
public:
	virtual ~ImGuiSystem();

	void update(World& world, float delta);

	virtual void registerImGui();
	inline virtual std::string name() { return "ImGuiSystem"; };
};
