// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "hovercomponent.hpp"

HoverComponent::HoverComponent(float hoverHeight, float hoverForce) {
	this->hoverHeight = hoverHeight;
	this->hoverForce = hoverForce;
}

HoverComponent::HoverComponent(const ComponentValues& value) : HoverComponent() {
	hoverHeight = value.getFloat("hoverHeight", 0.6);
	hoverForce = value.getFloat("hoverForce", 100);
}

HoverComponent::~HoverComponent() {}

void HoverComponent::registerImGui() {
	ImGui::DragFloat("Hover Height", &hoverHeight, 0.1, 0, 10);
	ImGui::DragFloat("Hover Force", &hoverForce, 1, 0, 100);
}
