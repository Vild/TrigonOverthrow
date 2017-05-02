#include "floortilecomponent.hpp"

FloorTileComponent::FloorTileComponent(float height)
{
	this->height = height;
}

FloorTileComponent::~FloorTileComponent()
{
}

float FloorTileComponent::getHeight()
{
	return height;
}

inline std::string FloorTileComponent::name()
{
	return "FloorTileComponent";
}

void FloorTileComponent::registerImGui()
{
}
