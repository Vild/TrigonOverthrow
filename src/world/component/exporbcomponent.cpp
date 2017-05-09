// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "exporbcomponent.hpp"

ExpOrbComponent::ExpOrbComponent() : giveExp(1) { }

ExpOrbComponent::ExpOrbComponent(const ComponentValues& value) : ExpOrbComponent() {
	this->giveExp = value.getInt("giveExp", 1);
}

ExpOrbComponent::~ExpOrbComponent() {}

void ExpOrbComponent::registerImGui() {}
