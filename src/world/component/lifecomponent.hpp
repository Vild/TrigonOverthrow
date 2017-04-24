#pragma once

#include "component.hpp"
#include "glm/glm.hpp"


struct LifeComponent : public Component {
	LifeComponent(int inMaxHP) { currHP = maxHP = inMaxHP; }
	LifeComponent() { currHP = maxHP = 4; };
	int maxHP;
	int currHP;

	void registerImGui() {};
	virtual std::string name() { return "GunComponent"; }
};