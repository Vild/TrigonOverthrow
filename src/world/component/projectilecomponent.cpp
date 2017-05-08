// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "projectilecomponent.hpp"

ProjectileComponent::ProjectileComponent(float damage) : damage(damage) {}

ProjectileComponent::ProjectileComponent(const ComponentValues& value) : ProjectileComponent() {
	damage = value.getFloat("damage", 0);
}

ProjectileComponent::~ProjectileComponent() {}

void ProjectileComponent::registerImGui() {}
