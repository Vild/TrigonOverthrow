#pragma once

#include "component.hpp"
#include "glm/glm.hpp"

struct LifeComponent : public Component {
	LifeComponent();
	LifeComponent(float inMaxHP);

	float maxHP;
	float currHP;
	bool hpchanged;
	void registerImGui() final;
	inline std::string name() final { return "LifeComponent"; }
};
