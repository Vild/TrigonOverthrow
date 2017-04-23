#pragma once
#include "component.hpp"
#include <glm/glm.hpp>

struct PhysicsComponent : public Component {
	glm::vec3 velocity = glm::vec3{0, 0, 0};
	glm::vec3 acceleration = glm::vec3{0, 0, 0};

	// Inherited via Component
	virtual void registerImGui();
	virtual std::string name() override { return "PhysicsComponent"; };
};
