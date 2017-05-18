// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "floortilecomponent.hpp"

FloorTileComponent::FloorTileComponent(float height) {
	this->height = height;
	this->heightFactor = 0.0f;
}

FloorTileComponent::FloorTileComponent(const ComponentValues& value) : FloorTileComponent() {
	height = value.getFloat("height", 1);
	heightFactor = value.getFloat("heightFactor", 0);
}

FloorTileComponent::~FloorTileComponent() {}

float FloorTileComponent::getHeight() {
	return height;
}

float FloorTileComponent::getHeightFactor() {
	return heightFactor;
}

void FloorTileComponent::setHeightFactor(float heightFactor) {
	this->heightFactor = heightFactor;
}

bool FloorTileComponent::lowerFloor(float delta)
{
	if (height > 0.0f)
		height -= delta;

	if (height < 0.0f)
		height = 0.0f;

	return height == 0.0f;
}

void FloorTileComponent::registerImGui() {}
