#pragma once
#include "component.hpp"
#include "glm/glm.hpp"

struct GunComponent : public Component {
	enum GunType : int { RAYGUN = 0, ENERGYGUN };

	GunType type;

	int cooldown;
	int cooldownLength;

	bool shoot;

	void addGun(GunType inType, int inCooldown);
	void registerImGui();
	virtual std::string name() { return "GunComponent"; }
};