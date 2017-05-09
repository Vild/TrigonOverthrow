// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "levelingcomponent.hpp"

LevelingComponent::LevelingComponent() : level(1), currentExp(0), upgradePoints(0) {
	nextExp = 0;
}

LevelingComponent::LevelingComponent(const ComponentValues& value) : LevelingComponent() {
	
}

LevelingComponent::~LevelingComponent() {}


void LevelingComponent::registerImGui() {}
