#pragma once
#include "component.hpp"
#include "modelcomponent.hpp"
#include "glm/glm.hpp"

struct GunComponent : public Component {
	enum GunType : int { RAYGUN = 0, ENERGYGUN };

	GunComponent() = default;
	GunComponent(const ComponentValues& value);
	virtual ~GunComponent();

	GunType type;
	float cooldown;
	float cooldownLength;
	bool shoot;

	void addGun(GunType inType, float inCooldown);
	void registerImGui() final;
	inline std::string name() final { return "GunComponent"; }
};
