#pragma once
#include "system.hpp"
#include "../component/transformcomponent.hpp"

class FloorTileSystem : public System {
public:
	FloorTileSystem();
	virtual ~FloorTileSystem();

	// Inherited via System
	void update(World& world, float delta) final;

	void registerImGui() final;

	inline std::string name() final { return "FloorTileSystem"; }
};
