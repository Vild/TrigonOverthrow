#pragma once
#include "component.hpp"

struct ExperienceOrbComponent : public Component{
	int amntOfExp;
	bool hasBeenPicked = false;

	ExperienceOrbComponent() = default;
	ExperienceOrbComponent(const ComponentValues& value);
	virtual ~ExperienceOrbComponent();
	inline std::string name() final { return "ExperienceOrbComponent"; }
	void registerImGui() final;
};