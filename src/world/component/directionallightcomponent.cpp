#include "directionallightcomponent.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <functional>

DirectionalLightComponent::DirectionalLightComponent()
{
	
}

void DirectionalLightComponent::recalculateProjectionMatrix()
{
	projection = glm::ortho(-size, size, -size, size, zNear, zFar);
}

std::string DirectionalLightComponent::name()
{
	return "DirectionalLightComponent";
}

void DirectionalLightComponent::registerImGui()
{
	bool dirty = false;

	ImGui::DragFloat3("Color", glm::value_ptr(color));
	dirty |= ImGui::DragFloat("Z Near", &zNear);
	dirty |= ImGui::DragFloat("Z Far", &zFar);
	dirty |= ImGui::DragFloat("Size", &size);

	
	if (dirty)
	{
		recalculateProjectionMatrix();
	}
}
