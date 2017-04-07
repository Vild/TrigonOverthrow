#pragma once
#include "system.hpp"
#include "../entity/entity.hpp"


class CameraSystem : public System
{
public:
	virtual void update(World & world, float delta) override;
};