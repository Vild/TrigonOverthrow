#pragma once
#include "component.hpp"
#include <glm/glm.hpp>

// Note! When making changes here add them also to FloorTransformComponent

struct ProjectileComponent : public Component {
public:
	ProjectileComponent(float damage);
	virtual ~ProjectileComponent();

	inline std::string name() final { return "ProjectileComponent"; }
	void registerImGui() final;

	float damage;
};
