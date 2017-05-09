#include "upgradecomponent.hpp"

UpgradeComponent::UpgradeComponent(const ComponentValues& value) : UpgradeComponent() {
	// TODO:
}

UpgradeComponent::~UpgradeComponent() {}

void UpgradeComponent::registerImGui() {
	if (ImGui::DragInt("RayMultiplier", &multipleRayMultiplier))
		multipleRayMultiplier = multipleRayMultiplier;
}