#pragma once

#include "component.hpp"

struct LightComponent : public Component {
	virtual std::string name() { return "LightComponent"; }
};
