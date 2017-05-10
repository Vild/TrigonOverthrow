#include "infiniteplanefollowcomponent.hpp"

InfinitePlaneFollowComponent::InfinitePlaneFollowComponent(TransformComponent * followTransform, float followHeight)
{
	this->followHeight = followHeight;
	this->followTransform = followTransform;
}

std::string InfinitePlaneFollowComponent::name()
{
	return "InfinitePlaneFollowComponent";
}

void InfinitePlaneFollowComponent::registerImGui()
{
	ImGui::DragFloat("Height", &followHeight);
}
