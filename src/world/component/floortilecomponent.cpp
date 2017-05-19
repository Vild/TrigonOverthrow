// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "floortilecomponent.hpp"

FloorTileComponent::FloorTileComponent(float height) {
	this->height = height;
	this->heightFactor = 0.0f;
	this->progress = 1.0f;
	this->state = STATE_DYNAMIC;
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

void FloorTileComponent::lowerFloor()
{
	state = STATE_LOWERING;
	progress = 0.0f;
}

void FloorTileComponent::staticFloor()
{
	state = STATE_STATIC;
	progress = 1.0f;
}

void FloorTileComponent::riseFloor()
{
	state = STATE_RISING;
	progress = 0.0f;
}

void FloorTileComponent::dynamicFloor()
{
	state = STATE_DYNAMIC;
	progress = 1.0f;
}

bool FloorTileComponent::isDone()
{
	return progress >= 1.0f;
}


FloorTileComponent::State FloorTileComponent::getState()
{
	return state;
}

float FloorTileComponent::getProgress()
{
	return progress;
}

void FloorTileComponent::setProgress(float progress)
{
	this->progress = progress;
}

void FloorTileComponent::registerImGui() {}
