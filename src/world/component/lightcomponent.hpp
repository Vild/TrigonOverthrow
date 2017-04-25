#pragma once

#include "component.hpp"

#include <glm/glm.hpp>

struct LightComponent : public Component {
	virtual ~LightComponent();

	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	virtual void registerImGui();
	virtual std::string name() { return "LightComponent"; }
};
