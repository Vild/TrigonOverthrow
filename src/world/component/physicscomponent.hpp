#pragma once
#include "component.hpp"
#include <glm/glm.hpp>

struct PhysicsComponent : public Component {
	glm::vec3 velocity = glm::vec3{0, 0, 0};
	glm::vec3 acceleration = glm::vec3{0, 0, 0};

	PhysicsComponent() = default;
	PhysicsComponent(const ComponentValues& value);
	virtual ~PhysicsComponent();

	// Inherited via Component
	void registerImGui() final;
	inline std::string name() final { return "PhysicsComponent"; };
};
