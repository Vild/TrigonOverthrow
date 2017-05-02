#include "floortilesystem.hpp"
#include "../component/floortilecomponent.hpp"
#include "../component/rigidbodycomponent.hpp"

#include <minmax.h>

FloorTileSystem::FloorTileSystem()
{
	this->playerTransform = nullptr;
}

FloorTileSystem::~FloorTileSystem()
{
}

void FloorTileSystem::setPlayerTransform(TransformComponent * playerTransform)
{
	this->playerTransform = playerTransform;
}

void FloorTileSystem::update(World & world, float delta)
{
	glm::vec3 playerPos = playerTransform->getPosition();
	for (auto& entity : world.getEntities())
	{
		FloorTileComponent* ftc = entity->getComponent<FloorTileComponent>();
		TransformComponent* tc = entity->getComponent<TransformComponent>();
		RigidBodyComponent* rbc = entity->getComponent<RigidBodyComponent>();

		if (ftc && tc)
		{
			float height = ftc->getHeight();
			glm::vec3 tilePos = tc->getPosition();
			float distance = glm::distance(playerPos, tilePos);


			float heightFactor =  1 - min(1, max(0.1, log2(max(0,distance - 3))));

			tc->setPosition({tilePos.x, height * heightFactor, tilePos.z});
			rbc->setTransform(tc);
		}
	}
}

void FloorTileSystem::registerImGui()
{
}

inline std::string FloorTileSystem::name()
{
	return "FloorTileSystem";
}
