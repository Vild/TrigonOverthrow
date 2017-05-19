// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "floortilesystem.hpp"
#include "../component/floortilecomponent.hpp"
#include "../component/rigidbodycomponent.hpp"

#include "../../engine.hpp"

FloorTileSystem::FloorTileSystem() {}

FloorTileSystem::~FloorTileSystem() {}

void FloorTileSystem::update(World& world, float delta)
{
	auto player = Engine::getInstance().getState().getPlayer();
	auto playerTransform = player->getComponent<TransformComponent>();
	if (!playerTransform)
		return;

	glm::vec3 playerPos = playerTransform->getPosition();

	for (Entity* entity : world.getActiveComponents<FloorTileComponent>()) {
		FloorTileComponent* ftc = entity->getComponent<FloorTileComponent>();
		TransformComponent* tc = entity->getComponent<TransformComponent>();
		RigidBodyComponent* rbc = entity->getComponent<RigidBodyComponent>();

		if (ftc && tc && rbc)
		{
			glm::vec3 tilePos = tc->getPosition();
			float heightFactor = ftc->getHeightFactor();
			float height = ftc->getHeight();
			float distance = glm::distance(playerPos, tilePos);
			float progress = ftc->getProgress();


			switch (ftc->getState())
			{
			case FloorTileComponent::STATE_DYNAMIC:
				heightFactor = max(heightFactor, 1.0f - clamp(log2(max(0, distance - 4.0f)) * 0.5f, 0.0f, 1.0f));
				break;

			case FloorTileComponent::STATE_LOWERING:
				progress += delta * 0.5f;
				heightFactor = min(heightFactor, glm::smoothstep(1.0f, 0.0f, progress));
				if (progress >= 1.0f) ftc->staticFloor();
				break;

			case FloorTileComponent::STATE_RISING:
				progress += delta * 0.5f;
				heightFactor = max(heightFactor, glm::smoothstep(0.0f, 1.0f, progress));
				if (progress >= 1.0f) ftc->staticFloor();
				break;
			default:
				break;
			}

			ftc->setProgress(progress);
			ftc->setHeightFactor(heightFactor);
			tc->setPosition({tilePos.x, height * heightFactor, tilePos.z});
			rbc->setTransform(tc);
		}
	}
}

void FloorTileSystem::registerImGui() {}

inline float FloorTileSystem::clamp(float v, float lo, float hi)
{
	if (v < lo)
		return lo;
	if (v > hi)
		return hi;
	return v;
}

inline float FloorTileSystem::max(float a, float b)
{
	if (a > b)
		return a;
	return b;
}

inline float FloorTileSystem::min(float a, float b)
{
	if (a < b)
		return a;
	return b;
}
