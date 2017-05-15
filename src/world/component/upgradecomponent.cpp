#include "upgradecomponent.hpp"

UpgradeComponent::UpgradeComponent(const ComponentValues& value) : UpgradeComponent() {
	// TODO:
}

UpgradeComponent::~UpgradeComponent() {}

void UpgradeComponent::registerImGui() {
	if (ImGui::DragInt("RayMultiplier", &multipleRayMultiplier));

	if (ImGui::DragInt("BounceCount", &reflectionCount));

	if (ImGui::DragInt("PierceCount", &refractionCount));

	if (ImGui::DragInt("UpgradePoints", &upgradePoints));
}