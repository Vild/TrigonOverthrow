#pragma once

#include "component.hpp"

#include <glm/glm.hpp>

struct KBMouseInputComponent : public Component {
	virtual ~KBMouseInputComponent();

	void registerImGui() final;
	inline std::string name() final { return "KBMouseInputComponent"; }
};
