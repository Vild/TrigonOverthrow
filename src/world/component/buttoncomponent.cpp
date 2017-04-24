#include "buttoncomponent.hpp"

#include <glm/gtc/type_ptr.hpp>

ButtonComponent::ButtonComponent() {}

ButtonComponent::~ButtonComponent() {}

void ButtonComponent::registerImGui() {
	ImGui::DragFloat3("Position", glm::value_ptr(position));
	ImGui::DragFloat3("Size", glm::value_ptr(size));
}
