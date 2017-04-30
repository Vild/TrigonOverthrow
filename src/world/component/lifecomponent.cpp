#include "lifecomponent.hpp"



void LifeComponent::registerImGui() {
	if (ImGui::DragFloat("Life", &currHP)) {
		currHP = currHP;
		hpchanged = true;
	}
}