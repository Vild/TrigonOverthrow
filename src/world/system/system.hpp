#pragma once

#include "../world.hpp"

class System {
public:
	virtual ~System() = 0;

	virtual void update(World& world, float delta) = 0;

	virtual void registerImGui() = 0;
	virtual std::string name() = 0;
};

inline System::~System() {}