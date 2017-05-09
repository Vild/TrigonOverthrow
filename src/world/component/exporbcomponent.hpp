#pragma once

#include "component.hpp"
#include "glm/glm.hpp"

struct ExpOrbComponent : public Component {
	ExpOrbComponent();
	ExpOrbComponent(const ComponentValues& value);
	virtual ~ExpOrbComponent();

	void registerImGui() final;
	inline std::string name() final { return "LifeComponent"; }
};
