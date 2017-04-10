#include "physicscomponent.hpp"
#include <glm\gtc\type_ptr.hpp>

void PhysicsComponent::registerImGui()
{
	ImGui::DragFloat3("Velocity", glm::value_ptr(velocity), 0);
	ImGui::DragFloat3("Acceleration", glm::value_ptr(acceleration), 0);
}