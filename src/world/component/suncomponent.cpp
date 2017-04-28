#include "suncomponent.hpp"

SunComponent::~SunComponent() {}

void SunComponent::registerImGui() {
	ImGui::ColorEdit3("Ambient", glm::value_ptr(ambient));

	ImGui::Text("Directional Light");
	ImGui::ColorEdit3("Diffuse", glm::value_ptr(directionLight.diffuse));
	ImGui::ColorEdit3("Specular", glm::value_ptr(directionLight.specular));
	ImGui::DragFloat3("Direction", glm::value_ptr(directionLight.direction), 0.1);
}