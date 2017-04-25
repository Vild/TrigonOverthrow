#pragma once

#include "component.hpp"
#include "glm/glm.hpp"


struct LifeComponent : public Component {
	LifeComponent(float inMaxHP) { currHP = maxHP = inMaxHP; }
	LifeComponent() { currHP = maxHP = 4; };
	float maxHP;
	float currHP;

	void registerImGui() {};
	virtual std::string name() { return "GunComponent"; }
};