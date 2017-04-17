#pragma once

#include <memory>
#include "../world/world.hpp"

class State {
public:
	//virtual void onEnter(State& prev);
	//virtual void onLeave(State& next);

	inline World& getWorld() { return _world; }

protected:
	World _world;
};
