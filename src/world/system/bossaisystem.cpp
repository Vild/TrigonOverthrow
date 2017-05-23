#include "bossaisystem.hpp"
#include "../component/bossaicomponent.hpp"
#include "../component/transformcomponent.hpp"
#include "../component/rigidbodycomponent.hpp"


BossAISystem::BossAISystem() {
	_walkToMiddle = false;
}

BossAISystem::~BossAISystem() {
	
}

void BossAISystem::update(World& world, float delta) {
	for (Entity* entity : world.getActiveComponents<BossAIComponent>()) {
		auto bossAIComp = entity->getComponent<BossAIComponent>();
		if (_walkToMiddle)
			_moveMiddle(entity);
	}
}

void BossAISystem::_moveMiddle(Entity* entity) {
	auto currPos = entity->getComponent<TransformComponent>()->getPosition();
	glm::vec3 direction = currPos - _centre;
	float distance = glm::distance(_centre, currPos);
	if (distance > 0) {
		auto rdbComp = entity->getComponent<RigidBodyComponent>();
		auto rdb = rdbComp->getRigidBody();
		rdb->applyCentralForce(rdbComp->getMass() * cast(direction));
	}else
		_walkToMiddle = false;

}

void BossAISystem::registerImGui() {
	
}