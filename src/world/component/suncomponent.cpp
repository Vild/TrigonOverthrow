// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "suncomponent.hpp"

SunComponent::SunComponent(const ComponentValues& value) : SunComponent() {
	ambient = value.getVec3("ambient", {0, 0, 0});
	directionLight.diffuse = value.getVec3("diffuse", {1, 1, 1});
	directionLight.specular = value.getVec3("specular", {0, 0, 0});
	directionLight.direction = value.getVec3("direction", {0, -1, 0});
}

SunComponent::~SunComponent() {}

void SunComponent::registerImGui() {
	ImGui::ColorEdit3("Ambient", glm::value_ptr(ambient));

	ImGui::Text("Directional Light");
	ImGui::ColorEdit3("Diffuse", glm::value_ptr(directionLight.diffuse));
	ImGui::ColorEdit3("Specular", glm::value_ptr(directionLight.specular));
	ImGui::DragFloat3("Direction", glm::value_ptr(directionLight.direction), 0.1);
}
