#pragma once
#include "system.hpp"

class LevelingSystem : public System {
public:
	void update(World& world, float delta) final;

	void registerImGui() final;
	inline std::string name() final { return "LevelingSystem"; };

private:
	int calcNextExp();
};
