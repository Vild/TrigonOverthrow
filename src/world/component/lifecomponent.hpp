#pragma once

#include "component.hpp"
#include "glm/glm.hpp"


struct LifeComponent : public Component {
	LifeComponent(float inMaxHP) { 
		currHP = maxHP = inMaxHP; 
		hpchanged = true;
	}
	LifeComponent() { currHP = maxHP = 4; };
	float maxHP;
	float currHP;
	bool hpchanged;
	void registerImGui();
	virtual std::string name() { return "LifeComponent"; }
};