#pragma once

#include "component.hpp"

#include <glm/glm.hpp>

struct KBMouseInputComponent : public Component<KBMouseInputComponent> {
	virtual std::string name() { return "KBMouseInputComponent"; }
};
