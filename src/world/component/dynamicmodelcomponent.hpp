#pragma once

#pragma once
#include "component.hpp"
#include "glm/glm.hpp"

struct DynamicModelComponent : public Component {


	void registerImGui();
	virtual std::string name() { return "GunComponent"; }
};