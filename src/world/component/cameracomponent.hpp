#pragma once
#include "component.hpp"
#include <glm/glm.hpp>
#include "../entity/entity.hpp"

struct CameraComponent : public Component<CameraComponent> {
	CameraComponent();

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

	glm::vec3 up = {0, 1, 0};
	float fov = 1.0f;
	float zNear = 0.1f;
	float zFar = 100.0f;
	float aspect = 1.77f;

	void lookAt(glm::vec3 eye, glm::vec3 center);
	void recalculateProjectionMatrix();

	virtual void registerImGui();

	virtual std::string name() { return "CameraComponent"; }
};
