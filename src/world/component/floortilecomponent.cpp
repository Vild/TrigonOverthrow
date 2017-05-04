// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "floortilecomponent.hpp"

FloorTileComponent::FloorTileComponent(float height) {
	this->height = height;
	this->heightFactor = 0.0f;
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

void FloorTileComponent::registerImGui() {}
