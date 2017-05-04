// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "floortilesystem.hpp"
#include "../component/floortilecomponent.hpp"
#include "../component/rigidbodycomponent.hpp"

FloorTileSystem::FloorTileSystem() {
	this->playerTransform = nullptr;
}

FloorTileSystem::~FloorTileSystem() {}

void FloorTileSystem::setPlayerTransform(TransformComponent* playerTransform) {
	this->playerTransform = playerTransform;
}

void FloorTileSystem::update(World& world, float delta) {
	static auto clamp = [](float v, float lo, float hi) -> float {
		if (v < lo)
			return lo;
		if (v > hi)
			return hi;
		return v;
	};

	static auto max = [](float a, float b) -> float {
		if (a > b)
			return a;
		return b;
	};

	glm::vec3 playerPos = playerTransform->getPosition();
	for (auto& entity : world.getEntities()) {
		FloorTileComponent* ftc = entity->getComponent<FloorTileComponent>();

		if (!ftc)
			continue;

		TransformComponent* tc = entity->getComponent<TransformComponent>();
		if (!tc)
			continue;

		float heightFactor = ftc->getHeightFactor();

		float height = ftc->getHeight();
		glm::vec3 tilePos = tc->getPosition();
		float distance = glm::distance(playerPos, tilePos);

		if (height <= 0 || distance >= 10)
			continue;

		heightFactor = max(heightFactor, 1.0f - clamp(log2(max(0, distance - 4.0f)) * 0.5f, 0.0f, 1.0f));

		ftc->setHeightFactor(heightFactor);
		tc->setPosition({tilePos.x, height * heightFactor, tilePos.z});

		RigidBodyComponent* rbc = entity->getComponent<RigidBodyComponent>();
		if (!rbc)
			continue;
		rbc->setTransform(tc);

		if (heightFactor == 1.f)
			entity->removeComponent<FloorTileComponent>();
	}
}

void FloorTileSystem::registerImGui() {}
