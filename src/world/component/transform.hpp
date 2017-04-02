#pragma once

#include "component.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

struct TransformComponent : public Component<TransformComponent> {
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;

	virtual void registerImGui() {
		ImGui::DragFloat3("Position", glm::value_ptr(position), 0.1);
		ImGui::DragFloat3("Velocity", glm::value_ptr(velocity), 0.1);
		ImGui::DragFloat3("Acceleration", glm::value_ptr(acceleration), 0.1);
	}
};