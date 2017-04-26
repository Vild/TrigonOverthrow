#pragma once

#include "../world.hpp"

#include "../../lib/Remotery.h"

class System {
public:
	virtual ~System() = 0;

	virtual void update(World& world, float delta) = 0;

	virtual void registerImGui() = 0;
	virtual std::string name() = 0;
};

inline System::~System() {}