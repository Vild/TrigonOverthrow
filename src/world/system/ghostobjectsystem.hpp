#pragma once
#include "system.hpp"

class GhostObjectSystem : public System {
public:
	GhostObjectSystem();
	virtual ~GhostObjectSystem();
	// Inherited functions from System.
	void update(World& world, float delta) final;
	void registerImGui() final;
	inline std::string name() final { return "GhostObjectSystem"; }

private:
};
