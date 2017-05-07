// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "pointlightcomponent.hpp"

PointLightComponent::PointLightComponent(const ComponentValues& value) : PointLightComponent() {
	pointLight.diffuse = value.getVec3("diffuse", {0, 0, 0});
	pointLight.specular = value.getVec3("specular", {0, 0, 0});
	pointLight.position = value.getVec3("position", {0, 0, 0});
	pointLight.constant = value.getFloat("constant", 1);
	pointLight.linear = value.getFloat("linear", 0.14);
	pointLight.quadratic = value.getFloat("quadratic", 0.07);
}

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
