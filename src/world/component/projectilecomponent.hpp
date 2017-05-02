#pragma once
#include "component.hpp"
#include <glm/glm.hpp>

// Note! When making changes here add them also to FloorTransformComponent

struct ProjectileComponent : public Component
{
public:
	ProjectileComponent(float damage) { this->damage = damage; }
	virtual ~ProjectileComponent() {};

	virtual void registerImGui() {};
	virtual std::string name() { return "ProjectileComponent"; }

	float damage;
};