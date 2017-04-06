#include "transformcomponent.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

void TransformComponent::updateMatrix()
{
	static const glm::mat4 identiy;
	matrix = glm::rotate(identiy, 1.0f, rotation);
	matrix = glm::scale(matrix, scale);
	matrix = glm::translate(matrix, position);
}

void TransformComponent::registerImGui() {
	ImGui::DragFloat3("Position", glm::value_ptr(position), 0.1);
	ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.1);
	ImGui::DragFloat4("Rotation", glm::value_ptr(rotation), 0.1);
}
