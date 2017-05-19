#pragma once
#include "component.hpp"

struct UpgradeComponent : public Component {
	static const int MAX_EXTRA_RAYS = 2;
	static const int MAX_REFLECT_LIMIT = 3;
	static const int MAX_REFRACTION_LIMIT = 3;

	int multipleRayMultiplier;
	int reflectionCount;
	int refractionCount;
	int upgradePoints;

	UpgradeComponent() = default;
	UpgradeComponent(const ComponentValues& value);
	virtual ~UpgradeComponent();
	inline std::string name() final { return "UpgradeComponent"; }
	void registerImGui() final;
};