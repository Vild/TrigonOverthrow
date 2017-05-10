#pragma once
#include "component.hpp"
#include <glm/glm.hpp>
#include "../entity.hpp"

struct CameraComponent : public Component {
	CameraComponent();
	CameraComponent(const ComponentValues& value);
	virtual ~CameraComponent();

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	float fov = 1.0f;
	float zNear = 1.f;
	float zFar = 25.f;
	float aspect = 1.77f;

	void recalculateProjectionMatrix();

	void registerImGui() final;

	inline std::string name() final { return "CameraComponent"; }
};
