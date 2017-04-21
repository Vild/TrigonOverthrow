#pragma once

#include <memory>
#include "../world/world.hpp"

class State {
public:
	virtual void onEnter(State* prev) = 0;
	virtual void onLeave(State* next) = 0;

	inline World& getWorld() { return _world; }
	virtual Entity* getCamera() = 0;

protected:
	World _world;
};
