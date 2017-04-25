#pragma once

#include "component.hpp"

struct LightComponent : public Component {
	virtual ~LightComponent();

	virtual std::string name() { return "LightComponent"; }
};
