// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "physicscomponent.hpp"
#include <glm/gtc/type_ptr.hpp>

PhysicsComponent::PhysicsComponent(const ComponentValues& value) : PhysicsComponent() {
	velocity = value.getVec3("velocity", {0, 0, 0});
	acceleration = value.getVec3("acceleration", {0, 0, 0});
}

PhysicsComponent::~PhysicsComponent() {}

void PhysicsComponent::registerImGui() {
	ImGui::DragFloat3("Velocity", glm::value_ptr(velocity), 0.1);
	ImGui::DragFloat3("Acceleration", glm::value_ptr(acceleration), 0.1);
}
