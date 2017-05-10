// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "levelingsystem.hpp"

#include <cmath>

void LevelingSystem::update(World& world, float delta) {
	for (std::unique_ptr<Entity>& entity : world.getEntities()) {
		auto levelingComp = entity->getComponent<LevelingComponent>();
		if (levelingComp)
		{
			if (levelingComp->currentExp >= levelingComp->nextExp)
			{
				levelingComp->level++;
				levelingComp->currentExp -= levelingComp->nextExp;
				levelingComp->nextExp = _calcNextExp(levelingComp);
			}
		}
	}
}

void LevelingSystem::registerImGui() {}

int LevelingSystem::_calcNextExp(LevelingComponent *comp) {

	int nextExp = 50 / 3 * (
		pow(comp->level, 3)
		- (6 * pow(comp->level, 2))
		+ (17 * comp->level)
		- 12
		);

	return nextExp;
}