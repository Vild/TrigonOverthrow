#pragma once

#include "component.hpp"

#include <glm/glm.hpp>

struct LightComponent : public Component {
	virtual ~LightComponent();

	// glm::vec3 pos;
	float radius;
	glm::vec3 color;
	float linear;
	float quadratic;
	float yaw;
	float pitch;

	virtual void registerImGui();
	virtual std::string name() { return "LightComponent"; }
};
