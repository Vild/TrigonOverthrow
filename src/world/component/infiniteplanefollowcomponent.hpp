#pragma once
#include "component.hpp"

class InfinitePlaneFollowComponent : public Component
{
public:


	// Inherited via Component
	virtual std::string name() override;

	virtual void registerImGui() override;

};