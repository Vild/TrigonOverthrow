#pragma once

#include "component.hpp"

#include <glm/glm.hpp>

struct KBMouseInputComponent : public Component<KBMouseInputComponent> {
	virtual void registerImGui();
	virtual std::string name() { return "KBMouseInputComponent"; }
};
