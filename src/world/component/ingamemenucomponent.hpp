#pragma once

#include "component.hpp"

struct InGameMenuComponent : public Component {
	InGameMenuComponent() = default;
	InGameMenuComponent(const ComponentValues& values);
	virtual ~InGameMenuComponent();

	typedef void (*renderUI_f)(Entity& self, State& state);
	renderUI_f renderUI;

	inline std::string name() final { return "InGameMenuComponent"; }
	void registerImGui() final;
};