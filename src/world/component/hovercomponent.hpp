#pragma once
#include "component.hpp"

class HoverComponent : public Component {
public:
	HoverComponent(float hoverHeight, float hoverForce);
	virtual ~HoverComponent();

	float hoverHeight;
	float hoverForce;

	// Inherited via Component
	inline std::string name() final { return "HoverComponent"; }
	void registerImGui() final;
};
