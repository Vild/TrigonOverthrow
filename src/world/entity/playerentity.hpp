#pragma once

#include "entity.hpp"

class PlayerEntity : public Entity {
public:
	PlayerEntity();

	virtual void registerImGui();
};
