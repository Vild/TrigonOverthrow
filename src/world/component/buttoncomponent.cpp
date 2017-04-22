#include "buttoncomponent.hpp"

#include <glm/gtc/type_ptr.hpp>

ButtonComponent::ButtonComponent() {}

void ButtonComponent::registerImGui() {
	ImGui::DragFloat2("Position", glm::value_ptr(position));
	ImGui::DragFloat2("Size", glm::value_ptr(size));
}
