#pragma once

#include <string>
#include <vector>
#include <memory>

#include "../../lib/imgui.h"
#include "../../lib/Remotery.h"

/// Used to identifying a Component
struct Component {
	virtual ~Component() = 0;

	virtual std::string name() = 0;

	virtual void registerImGui() = 0;
};

inline Component::~Component() {}