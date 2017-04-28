#pragma once

#include "component.hpp"

#include "../renderpass/lightingrenderpass.hpp"

/// Aka DirectionLight
struct SunComponent : public Component {
	glm::vec3 ambient;
	DirLight directionLight;

	virtual ~SunComponent();

	virtual void registerImGui();
	virtual std::string name() { return "SunComponent"; }
};