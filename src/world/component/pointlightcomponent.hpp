#pragma once

#include "component.hpp"

#include <glm/glm.hpp>

#include "../renderpass/lightingrenderpass.hpp"

struct PointLightComponent : public Component {
	PointLight pointLight;

	glm::vec3 offset;

	virtual ~PointLightComponent();

	virtual void registerImGui();
	virtual std::string name() { return "PointLightComponent"; }
};
