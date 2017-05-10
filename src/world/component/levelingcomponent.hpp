#pragma once

#include "component.hpp"
#include "glm/glm.hpp"

struct LevelingComponent : public Component {
	LevelingComponent();
	LevelingComponent(const ComponentValues& value);
	virtual ~LevelingComponent();

	int level;
	int currentExp;
	int nextExp;
	int upgradePoints;

	void registerImGui() final;
	inline std::string name() final { return "LevelingComponent"; }
};
