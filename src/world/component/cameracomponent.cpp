#include "cameracomponent.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>



CameraComponent::CameraComponent()
{
	recalculateProjectionMatrix();
}

void CameraComponent::recalculateProjectionMatrix()
{
	projectionMatrix = glm::perspective(fov, aspect, zNear, zFar);
}

void CameraComponent::registerImGui() {
	bool dirty = false;

	dirty |= ImGui::DragFloat("Fov", &fov, 0.1);
	dirty |= ImGui::DragFloat("zNear", &zNear, 0.1);
	dirty |= ImGui::DragFloat("zFar", &zFar, 0.1);
	dirty |= ImGui::DragFloat("Aspect", &aspect, 0.1);

	if (dirty)
	{
		recalculateProjectionMatrix();
	}
}
