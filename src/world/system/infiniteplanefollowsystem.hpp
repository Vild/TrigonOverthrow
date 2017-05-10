#pragma once
#include "system.hpp"

class InfinitePlaneFollowSystem : public System
{
public:


	// Inherited via System
	virtual void update(World & world, float delta) override;

	virtual void registerImGui() override;

	virtual std::string name() override;

};