#include "lookat.hpp"

void LookAtComponent::registerImGui() {
	ImGui::Columns(2);
	if (target)
		ImGui::Text(target->getName().c_str());
	else
		ImGui::Text("**NULL**");

	ImGui::NextColumn();
	ImGui::Text("Targeting");
	ImGui::NextColumn();
	ImGui::Columns(1);

	ImGui::DragFloat("Min distance", &minDistance);
	ImGui::DragFloat("Max distance", &maxDistance);
}
