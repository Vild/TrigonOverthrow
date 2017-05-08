#pragma once
#include "component.hpp"

class HoverComponent : public Component {
public:
	HoverComponent() = default;
	HoverComponent(float hoverHeight, float hoverForce);
	HoverComponent(const ComponentValues& value);
	virtual ~HoverComponent();

	float hoverHeight;
	float hoverForce;

	// Inherited via Component
	inline std::string name() final { return "HoverComponent"; }
	void registerImGui() final;
};
