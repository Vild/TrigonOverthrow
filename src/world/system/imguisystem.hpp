#pragma once

#include "system.hpp"

class ImGuiSystem : public System {
public:
	virtual ~ImGuiSystem();

	void update(World& world, float delta) final;

	void registerImGui() final;
	inline std::string name() final { return "ImGuiSystem"; };
};
