#pragma once

#include "component.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

struct TransformComponent : public Component<TransformComponent> {
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;

	glm::vec3 scale;
	glm::mat4 rotation;

	virtual void registerImGui();

	virtual std::string name() { return "TransformComponent"; }
};
