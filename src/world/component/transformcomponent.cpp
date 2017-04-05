#include "transformcomponent.hpp"

void TransformComponent::registerImGui() {
	ImGui::DragFloat3("Position", glm::value_ptr(position), 0.1);
	ImGui::DragFloat3("Velocity", glm::value_ptr(velocity), 0.1);
	ImGui::DragFloat3("Acceleration", glm::value_ptr(acceleration), 0.1);
	ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.1);

	ImGui::DragFloat4("Rotation[0]", glm::value_ptr(rotation[0]), 0.1);
	ImGui::DragFloat4("Rotation[1]", glm::value_ptr(rotation[1]), 0.1);
	ImGui::DragFloat4("Rotation[2]", glm::value_ptr(rotation[2]), 0.1);
	ImGui::DragFloat4("Rotation[3]", glm::value_ptr(rotation[3]), 0.1);
}
