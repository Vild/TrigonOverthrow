// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "guncomponent.hpp"

GunComponent::~GunComponent() {}

void GunComponent::addGun(GunComponent::GunType inType, int inCooldown) {
	type = inType;
	cooldownLength = inCooldown;
	cooldown = 0;
	shoot = false;
	switch (inType) {
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