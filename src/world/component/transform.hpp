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

	virtual void registerImGui() {
		ImGui::DragFloat3("Position", glm::value_ptr(position), 0.1);
		ImGui::DragFloat3("Velocity", glm::value_ptr(velocity), 0.1);
		ImGui::DragFloat3("Acceleration", glm::value_ptr(acceleration), 0.1);
		ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.1);

		ImGui::DragFloat4("Rotation[0]", glm::value_ptr(rotation[0]), 0.1);
		ImGui::DragFloat4("Rotation[1]", glm::value_ptr(rotation[1]), 0.1);
		ImGui::DragFloat4("Rotation[2]", glm::value_ptr(rotation[2]), 0.1);
		ImGui::DragFloat4("Rotation[3]", glm::value_ptr(rotation[3]), 0.1);
	}

	virtual std::string name() { return "TransformComponent"; }
};
