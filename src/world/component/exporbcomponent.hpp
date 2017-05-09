#pragma once

#include "component.hpp"
#include "glm/glm.hpp"

struct ExpOrbComponent : public Component {
	ExpOrbComponent();
	ExpOrbComponent(const ComponentValues& value);
	virtual ~ExpOrbComponent();

	int giveExp;

	void registerImGui() final;
	inline std::string name() final { return "ExpOrbComponent"; }
};
