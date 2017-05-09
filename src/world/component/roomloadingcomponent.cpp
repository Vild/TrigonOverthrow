#include "roomloadingcomponent.hpp"
#include "../entity.hpp"

RoomLoadingComponent::RoomLoadingComponent(glm::ivec2 coord)
{
	this->coord = coord;
}

RoomLoadingComponent::~RoomLoadingComponent()
{
}

void RoomLoadingComponent::unload()
{
	for (Entity * entity : entities)
	{
		entity->makeDead();
	}
}

void RoomLoadingComponent::addEntity(Entity * entity)
{
	entities.push_back(entity);
}

std::string RoomLoadingComponent::name()
{
	return "RoomLoadingComponent";
}

void RoomLoadingComponent::registerImGui()
{
}
