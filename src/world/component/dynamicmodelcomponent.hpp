#pragma once

#pragma once
#include "component.hpp"
#include "glm/glm.hpp"

struct DynamicModelComponent : public Component {
	DynamicModelComponent(float hp);
	virtual ~DynamicModelComponent() {}

	void registerImGui();
	virtual std::string name() { return "GunComponent"; }


};