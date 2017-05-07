// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "lifecomponent.hpp"

LifeComponent::LifeComponent() : maxHP(4), currHP(4), hpchanged(false) {}

LifeComponent::LifeComponent(float inMaxHP) : maxHP(inMaxHP), currHP(inMaxHP), hpchanged(true) {}

LifeComponent::LifeComponent(const ComponentValues& value) : LifeComponent() {
	maxHP = value.getFloat("maxHP", 4);
	currHP = value.getFloat("currHP", 4);
	hpchanged = value.getBool("hpchanged", false);
}

LifeComponent::~LifeComponent() {}

void LifeComponent::registerImGui() {
	if (ImGui::DragFloat("Life", &currHP)) {
		currHP = currHP;
		hpchanged = true;
	}
}
