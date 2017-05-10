#pragma once
#include "component.hpp"
#include <glm/glm.hpp>

struct ProjectileComponent : public Component {
public:
	ProjectileComponent() = default;
	ProjectileComponent(float damage);
	ProjectileComponent(const ComponentValues& value);
	virtual ~ProjectileComponent();

	inline std::string name() final { return "ProjectileComponent"; }
	void registerImGui() final;

	float damage;
	int bounceCount;
	int pierceCount;
};
