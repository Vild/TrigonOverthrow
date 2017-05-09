// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "levelingsystem.hpp"
#include "../component/levelingcomponent.hpp"

#include <cmath>

void LevelingSystem::update(World& world, float delta) {
	for (std::unique_ptr<Entity>& entity : world.getEntities()) {
		auto levelingComp = entity->getComponent<LevelingComponent>();
		if (levelingComp)
		{


		}
	}
}

void LevelingSystem::registerImGui() {}

int LevelingSystem::calcNextExp() {



}