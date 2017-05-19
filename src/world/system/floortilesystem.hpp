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
private:
	static float clamp(float v, float lo, float hi);;

	static float max(float a, float b);
	static float min(float a, float b);
};
