#include "floortilecomponent.hpp"

FloorTileComponent::FloorTileComponent(float height)
{
	this->height = height;
	this->heightFactor = 0.0f;
}

FloorTileComponent::~FloorTileComponent()
{
}

float FloorTileComponent::getHeight()
{
	return height;
}

float FloorTileComponent::getHeightFactor()
{
	return heightFactor; 
}

void FloorTileComponent::setHeightFactor(float heightFactor) 
{ 
	this->heightFactor = heightFactor; 
}

inline std::string FloorTileComponent::name()
{
	return "FloorTileComponent";
}

void FloorTileComponent::registerImGui()
{
}
