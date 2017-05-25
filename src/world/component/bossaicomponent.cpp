#include "bossaicomponent.hpp"
#include "rigidbodycomponent.hpp"
#include "../system/bulletphysicssystem.hpp"
#include "../../engine.hpp"

BossAIComponent::BossAIComponent(const ComponentValues& value) {
	usefulTimer = 0;
	doInverse = false;
	currState = BossStates::monologue;
	monologueTimer = 6;
	currentBatch = 0;
	//auto loader = Engine::getInstance().getJSONLoader();
	//auto world = Engine::getInstance().getState().getWorld();
	//auto physWorld = Engine::getInstance().getSystem<BulletPhysicsSystem>();
	//const std::string filePath = "assets/entities/trigon_projectile.json";
	//for (int i = 0; i < 100; i++) {
	//	projectiles.push_back(loader->constructEntity(Engine::getInstance().getState().getWorld(), filePath, json()));
	//	auto newRdb = projectiles[i]->getComponent<RigidBodyComponent>();
	//	newRdb->setActivationState(DISABLE_DEACTIVATION);
	//	physWorld->addRigidBody(newRdb, BulletPhysicsSystem::CollisionType::COL_ENEMY_PROJECTILE,
	//		BulletPhysicsSystem::enemyProjectileCollidesWith);
	//}
}

BossAIComponent::~BossAIComponent() {
	
}

void BossAIComponent::registerImGui() {
	
}