#pragma once

#include "component.hpp"

#include "../renderpass/lightingrenderpass.hpp"

/// Aka DirectionLight
struct SunComponent : public Component {
	glm::vec3 ambient;
	DirLight directionLight;

	SunComponent() = default;
	SunComponent(const ComponentValues& value);
	virtual ~SunComponent();

	void registerImGui() final;
	inline std::string name() final { return "SunComponent"; }
};
