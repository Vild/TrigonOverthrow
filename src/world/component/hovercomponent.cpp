#include "hovercomponent.hpp"

HoverComponent::HoverComponent(float hoverHeight, float hoverForce)
{
	this->hoverHeight = hoverHeight;
	this->hoverForce = hoverForce;
}

HoverComponent::~HoverComponent()
{

}

std::string HoverComponent::name()
{
	return "HoverComponent";
}

void HoverComponent::registerImGui()
{
	ImGui::DragFloat("Hover Height", &hoverHeight, 0.1, 0, 10);
	ImGui::DragFloat("Hover Force", &hoverForce, 1, 0, 100);
}
