// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "physicscomponent.hpp"
#include <glm/gtc/type_ptr.hpp>

PhysicsComponent::~PhysicsComponent() {}

void PhysicsComponent::registerImGui() {
	ImGui::DragFloat3("Velocity", glm::value_ptr(velocity), 0.1);
	ImGui::DragFloat3("Acceleration", glm::value_ptr(acceleration), 0.1);
}
