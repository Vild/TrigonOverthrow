#pragma once
#include "component.hpp"

class HoverComponent : public Component
{
public:
	HoverComponent(float hoverHeight, float hoverForce);
	virtual ~HoverComponent();

	float hoverHeight;
	float hoverForce;

	// Inherited via Component
	virtual std::string name() override;
	virtual void registerImGui() override;
};