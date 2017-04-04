#pragma once

#include "component.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

struct TransformComponent : public Component<TransformComponent> {
	glm::vec3 position = glm::vec3{0, 0, 0};
	glm::vec3 velocity = glm::vec3{0, 0, 0};
	glm::vec3 acceleration = glm::vec3{0, 0, 0};

	glm::vec3 scale = glm::vec3{1, 1, 1};
	glm::mat4 rotation = glm::mat4();

	virtual void registerImGui();

	virtual std::string name() { return "TransformComponent"; }
};
