#pragma once
#include "component.hpp"

struct AIComponent: public Component {
	AIComponent() = default;
	AIComponent(const ComponentValues& value);
	virtual ~AIComponent();
	inline std::string name() final { return "AIComponent"; }
	void registerImGui() final;
};