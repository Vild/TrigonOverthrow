// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "lifesystem.hpp"
#include "../component/lifecomponent.hpp"
#include "../component/transformcomponent.hpp"
#include "../component/particlecomponent.hpp"
#include "../component/projectilecomponent.hpp"
#include "../component/rigidbodycomponent.hpp"
#include "../component/aicomponent.hpp"
#include "../component/bossaicomponent.hpp"
#include "../system/bulletphysicssystem.hpp"
#include "../system/roomloadingsystem.hpp"
#include "../component/guncomponent.hpp"
#include "../../io/jsonloader.hpp"
#include "../../engine.hpp"

#include "../../state/winstate.hpp"

void LifeSystem::update(World& world, float delta) {
	for (Entity* entity : world.getActiveComponents<LifeComponent>()) {
		auto lifeComp = entity->getComponent<LifeComponent>();
		if (!lifeComp)
			continue;

		if (lifeComp->currHP <= 0) {
			entity->makeDead();
			if (!entity->getComponent<ProjectileComponent>() && !entity->getComponent<ParticleComponent>()) {
				const std::string filePath = "assets/entities/experienceOrb.json";
				auto newEntity = Engine::getInstance().getJSONLoader()->constructEntity(world, filePath.c_str(), json());
				auto newTransComp = newEntity->getComponent<TransformComponent>();
				auto oldTransComp = entity->getComponent<TransformComponent>();
				auto newParticleComp = newEntity->getComponent<ParticleComponent>();
				auto rdbComp = newEntity->getComponent<RigidBodyComponent>();

				newTransComp->setPosition(oldTransComp->getPosition());
				rdbComp->setTransform(newTransComp);
				rdbComp->setHitboxHalfSize(oldTransComp->getScale());
				rdbComp->setActivationState(DISABLE_DEACTIVATION);
				newParticleComp->addEmitter(oldTransComp->getPosition(), glm::vec3(0, 1, 0), ParticleComponent::ParticleEffect::ORB);
				Engine::getInstance().getSystem<BulletPhysicsSystem>()->addRigidBody(rdbComp, BulletPhysicsSystem::CollisionType::COL_EXP_ORB,
																																						 BulletPhysicsSystem::orbCollidesWith);

				Engine::getInstance().getSystem<RoomLoadingSystem>()->enemyDead(world, entity);
			}
		}

		if ((entity->getComponent<ProjectileComponent>() || entity->getComponent<ParticleComponent>()) && !entity->getComponent<AIComponent>() &&
				!entity->getComponent<BossAIComponent>()) {
			lifeComp->currHP -= 1 * delta;
		}

		if (entity->getComponent<BossAIComponent>() && !lifeComp->currHP)
			Engine::getInstance().setState<WinState>();
	}
}

void LifeSystem::registerImGui() {}