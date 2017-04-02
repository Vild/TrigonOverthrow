#pragma once

#include "component.hpp"

struct LightComponent : public Component<LightComponent> {
	virtual std::string name() { return "LightComponent"; }
};
