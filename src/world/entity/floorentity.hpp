#pragma once

#include "entity.hpp"

class FloorEntity : public Entity {
public:
	FloorEntity();

	virtual void registerImGui();
};
