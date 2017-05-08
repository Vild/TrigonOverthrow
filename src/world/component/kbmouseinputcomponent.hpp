#pragma once

#include "component.hpp"

#include <glm/glm.hpp>

struct KBMouseInputComponent : public Component {
	KBMouseInputComponent() = default;
	KBMouseInputComponent(const ComponentValues& value);
	virtual ~KBMouseInputComponent();

	void registerImGui() final;
	inline std::string name() final { return "KBMouseInputComponent"; }
};
