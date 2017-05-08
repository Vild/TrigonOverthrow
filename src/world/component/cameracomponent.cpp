// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "cameracomponent.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

CameraComponent::CameraComponent() {
	recalculateProjectionMatrix();
}

CameraComponent::CameraComponent(const ComponentValues& value) : CameraComponent() {
	fov = value.getFloat("fov", 1.0f);
	zNear = value.getFloat("zNear", 0.001f);
	zFar = value.getFloat("zFar", 100.0f);
	aspect = value.getFloat("aspect", 1.77f);
	recalculateProjectionMatrix();
}

CameraComponent::~CameraComponent() {}

void CameraComponent::recalculateProjectionMatrix() {
	projectionMatrix = glm::perspective(fov, aspect, zNear, zFar);
}

void CameraComponent::registerImGui() {
	bool dirty = false;

	dirty |= ImGui::DragFloat("Fov", &fov, 0.1f);
	dirty |= ImGui::DragFloat("zNear", &zNear, 0.1f);
	dirty |= ImGui::DragFloat("zFar", &zFar, 0.1f);
	dirty |= ImGui::DragFloat("Aspect", &aspect, 0.1f);

	if (dirty)
		recalculateProjectionMatrix();
}
