#include "transformcomponent.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>

void TransformComponent::recalculateMatrix()
{
	matrix = glm::translate(position) * glm::scale(scale) * glm::rotate(1.0f, rotation);
}

glm::vec3 TransformComponent::getDirection()
{
	glm::vec3 direction;

	float pitch = glm::radians(rotation.x);
	float yaw = glm::radians(rotation.y);

	direction.x = cos(pitch) * cos(yaw);
	direction.y = sin(pitch);
	direction.z = cos(pitch) * sin(yaw);

	return normalize(direction);
}

void TransformComponent::registerImGui() {

	bool dirty = false;

	dirty |= ImGui::DragFloat3("Position", glm::value_ptr(position), 0.1);
	dirty |= ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.1);
	dirty |= ImGui::DragFloat3("Rotation", glm::value_ptr(rotation), 0.1);


#ifndef DAN
	glm::mat4 tmatrix = glm::transpose(matrix);
#else
	glm::mat4 tmatrix = matrix;
#endif // !DAN


	dirty |= ImGui::DragFloat4("Matrix[0]", glm::value_ptr(tmatrix[0]));
	dirty |= ImGui::DragFloat4("Matrix[1]", glm::value_ptr(tmatrix[1]));
	dirty |= ImGui::DragFloat4("Matrix[2]", glm::value_ptr(tmatrix[2]));
	dirty |= ImGui::DragFloat4("Matrix[3]", glm::value_ptr(tmatrix[3]));

	if (dirty)
	{
		recalculateMatrix();
	}
}
