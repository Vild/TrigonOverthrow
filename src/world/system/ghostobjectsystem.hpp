#pragma once
#include "system.hpp"

class GhostObjectSystem : public System {
public:
	GhostObjectSystem();
	virtual ~GhostObjectSystem();
	// Inherited functions from System.
	virtual void update(World& world, float delta);
	virtual void registerImGui();
	inline virtual std::string name() { return "GhostObjectSystem"; }
private:

};