#include <cmath>
#include "bossaisystem.hpp"
#include "../component/bossaicomponent.hpp"
#include "../component/transformcomponent.hpp"
#include "../component/rigidbodycomponent.hpp"
#include "../component/guncomponent.hpp"
#include "../component/lifecomponent.hpp"
#include <glm/gtx/transform.hpp>

BossAISystem::BossAISystem() {
	_walkToMiddle = false;
	_walkDistance = M_PI * 2.f;
}

BossAISystem::~BossAISystem() {

}

void BossAISystem::update(World& world, float delta) {
	for (Entity* entity : world.getActiveComponents<BossAIComponent>()) {
		auto gunComp = entity->getComponent<GunComponent>();
		if (gunComp->cooldown <= 0)
			gunComp->shoot = true;
		_doPhase(entity, delta);
	}
}

void BossAISystem::_moveMiddle(Entity* entity) {
	auto currPos = entity->getComponent<TransformComponent>()->getPosition();
	auto bossAIComp = entity->getComponent<BossAIComponent>();
	glm::vec3 direction = currPos - _centre;
	float distance = glm::distance(_centre, currPos);
	if (distance > 0) {
		_walkToMiddle = true;
		auto rdbComp = entity->getComponent<RigidBodyComponent>();
		auto rdb = rdbComp->getRigidBody();
		rdb->applyCentralForce(rdbComp->getMass() * cast(direction));
	}
	else {
		_walkToMiddle = false;
		bossAIComp->dir *= -1;
	}

}

void BossAISystem::_doPhase(Entity* entity, float delta) {
	auto bossAIComp = entity->getComponent<BossAIComponent>();
	auto currState = bossAIComp->currState;
	auto rdbComp = entity->getComponent<RigidBodyComponent>();
	auto rigidbody = rdbComp->getRigidBody();
	auto transComp = entity->getComponent<TransformComponent>();
	auto lifeComp = entity->getComponent<LifeComponent>();
	if (bossAIComp->usefulTimer >= _walkDistance && currState != BossAIComponent::BossStates::monologue) {
		bossAIComp->dir *= -1;
		bossAIComp->usefulTimer = 0;
		_walkDistance = M_PI * 4;
	}
	switch (currState) {
	case BossAIComponent::BossStates::monologue:
		if (bossAIComp->monologueTimer <= 0) {
			bossAIComp->currState = BossAIComponent::BossStates::firstPhase;
			rdbComp->setActivationState(DISABLE_DEACTIVATION);
		}
		bossAIComp->monologueTimer -= 1 * delta;
		break;
	case BossAIComponent::BossStates::firstPhase:
		transComp->setRotation(glm::quat_cast(glm::rotate(1.f * bossAIComp->usefulTimer, glm::vec3(0, 1, 0))));
		rigidbody->applyCentralForce(cast(bossAIComp->dir) * 10.f);
		if (lifeComp->currHP <= lifeComp->maxHP - 5) {
			entity->getComponent<GunComponent>()->cooldownLength *= 0.5f;
			bossAIComp->currState = BossAIComponent::BossStates::finalPhase;
		}
		bossAIComp->usefulTimer += 1 * delta;
		break;
	case BossAIComponent::BossStates::finalPhase:
		transComp->setRotation(glm::quat_cast(glm::rotate(1.f * bossAIComp->usefulTimer, glm::vec3(0, 1, 0))));
		rigidbody->applyCentralForce(cast(bossAIComp->dir) * 10.f);
		rigidbody->applyCentralForce(cast(_calculateForceDirection(bossAIComp->usefulTimer)) * 10.f);
		bossAIComp->usefulTimer += 1 * delta;
		break;
	default:
		break;
	}
}

glm::vec3 BossAISystem::_calculateForceDirection(float time) {
	glm::vec3 newDir = glm::vec3(0);
	glm::vec3 forward = glm::vec3(0, 0, -1);
	glm::vec3 right = glm::cross(forward, glm::vec3(0, 1, 0));
	newDir = glm::vec3(right.x * sin(time), 0, forward.z * cos(time));
	return newDir;
}
void BossAISystem::registerImGui() {

}