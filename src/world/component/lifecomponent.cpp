// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "lifecomponent.hpp"

#include <cstdio>

LifeComponent::LifeComponent() : maxHP(4), currHP(4), hpchanged(true) {}

LifeComponent::LifeComponent(float inMaxHP) : maxHP(inMaxHP), currHP(inMaxHP), hpchanged(true) {}

LifeComponent::LifeComponent(const ComponentValues& value) : LifeComponent() {
	currHP = maxHP = value.getFloat("maxHP", 4);
	hpchanged = true;
}

LifeComponent::~LifeComponent() {}

void LifeComponent::registerImGui() {
	if (ImGui::DragFloat("Life", &currHP)) {
		currHP = currHP;
		hpchanged = true;
	}
}
