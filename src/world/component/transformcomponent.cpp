#include "transformcomponent.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>

void TransformComponent::recalculateMatrix()
{
	static const glm::vec3 x(1, 0, 0);
	static const glm::vec3 y(0, 1, 0);
	static const glm::vec3 z(0, 0, 1);

	matrix = glm::translate(position)
		   * glm::rotate(glm::radians(rotation.z), z)
		   * glm::rotate(glm::radians(rotation.y), y)
		   * glm::rotate(-glm::radians(rotation.x), x)
		   * glm::scale(scale);
}

glm::vec3 TransformComponent::getDirection()
{
	static const glm::vec3 x(1, 0, 0);
	static const glm::vec3 y(0, 1, 0);
	static const glm::vec3 z(0, 0, 1);

	glm::vec4 direction(0, 0, 1, 0);
	direction = glm::rotate(glm::radians(rotation.z), z)
			  * glm::rotate(glm::radians(rotation.y), y)
			  * glm::rotate(-glm::radians(rotation.x), x)
			  * direction;

	return glm::vec3(direction);
}

void TransformComponent::registerImGui() {

	bool dirty = false;

	dirty |= ImGui::DragFloat3("Position", glm::value_ptr(position), 0.1);
	dirty |= ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.1);
	dirty |= ImGui::DragFloat3("Rotation", glm::value_ptr(rotation), 0.1);

	if (dirty)
	{
		recalculateMatrix();
	}
}
