// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "pointlightcomponent.hpp"

PointLightComponent::~PointLightComponent() {}

void PointLightComponent::registerImGui() {
	ImGui::ColorEdit3("Diffuse", glm::value_ptr(pointLight.diffuse));
	ImGui::ColorEdit3("Specular", glm::value_ptr(pointLight.specular));
	ImGui::DragFloat3("Position", glm::value_ptr(pointLight.position), 0.1);
	ImGui::DragFloat("Constant", &pointLight.constant, 0, 0, 1);
	ImGui::DragFloat("Linear", &pointLight.linear, 0.01);
	ImGui::DragFloat("Quadratic", &pointLight.quadratic, 0.001);
	ImGui::Text(" ");
	ImGui::DragFloat3("Offset", glm::value_ptr(offset), 0.1);
}