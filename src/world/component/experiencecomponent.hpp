#pragma once
#include "component.hpp"
struct ExperienceComponent : public Component {
	int currExp;
	int expToNextLevel;

	ExperienceComponent() = default;
	ExperienceComponent(const ComponentValues& value);
	virtual ~ExperienceComponent();
	inline std::string name() final { return "ExperienceComponent"; }
	void registerImGui() final;
};
