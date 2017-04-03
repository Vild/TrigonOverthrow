#pragma once

#include "component.hpp"

#include <glm/glm.hpp>

struct KBMouseInputComponent : public Component<KBMouseInputComponent> {
	glm::ivec2 xy;
	glm::ivec2 xyDiff;
	glm::vec3 direction;

	// TODO: Fix
	void update();

	virtual std::string name() { return "KBMouseInputComponent"; }
};
