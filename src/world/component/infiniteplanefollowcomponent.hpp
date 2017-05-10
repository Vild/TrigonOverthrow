#pragma once
#include "component.hpp"
#include "transformcomponent.hpp"

class InfinitePlaneFollowComponent : public Component
{
public:
	InfinitePlaneFollowComponent(TransformComponent * followTransform, float followHeight);

	TransformComponent * followTransform;
	float followHeight = 0.001;

	// Inherited via Component
	virtual std::string name() override;

	virtual void registerImGui() override;

};