#pragma once
#include "component.hpp"

struct UpgradeComponent : public Component {
	enum UpgradeType : int {multiplerays = 0, reflectiverays = 0, refractionrays = 0};
	
	UpgradeType upgrades[3];
	const int MAX_EXTRA_RAYS = 2;
	const int MAX_REFLECT_LIMIT = 3;
	const int MAX_REFRACTION_LIMIT = 3;
	int multipleRayMultiplier;
	int reflectionCount;
	int refractionCount;

	UpgradeComponent() = default;
	UpgradeComponent(const ComponentValues& value);
	virtual ~UpgradeComponent();
	inline std::string name() final { return "UpgradeComponent"; }
	void registerImGui() final;
};