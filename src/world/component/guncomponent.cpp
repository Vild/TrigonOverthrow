#include "guncomponent.hpp"

GunComponent::~GunComponent() {}

void GunComponent::addGun(GunComponent::GunType inType, int inCooldown) {
	type = inType;
	cooldownLength = inCooldown;
	cooldown = 0;
	shoot = false;
	switch (inType)
	{
	case GunComponent::RAYGUN: {
		
		break;
	}
	case GunComponent::ENERGYGUN: {
			
		break;
	}
	default:
		break;
	}
}

void GunComponent::registerImGui() {}