#pragma once

#include "component.hpp"
#include "glm/glm.hpp"

struct LifeComponent : public Component {
	LifeComponent();
	LifeComponent(float inMaxHP);
	LifeComponent(const ComponentValues& value);
	virtual ~LifeComponent();

	float maxHP;
	float currHP;
	bool hpchanged = true;
	void registerImGui() final;
	inline std::string name() final { return "LifeComponent"; }
};
