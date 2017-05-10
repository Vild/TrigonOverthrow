#pragma once
#include "system.hpp"
#include "../component/levelingcomponent.hpp"

class LevelingSystem : public System {
public:
	void update(World& world, float delta) final;

	void registerImGui() final;
	inline std::string name() final { return "LevelingSystem"; };

private:
	int _calcNextExp(LevelingComponent *comp);
};
