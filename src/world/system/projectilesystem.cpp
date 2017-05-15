// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "projectilesystem.hpp"
#include "bulletphysicssystem.hpp"
#include "../component/projectilecomponent.hpp"
#include "../component/particlecomponent.hpp"
#include "../component/lifecomponent.hpp"
#include "../component/floortilecomponent.hpp"
#include "../../engine.hpp"
#include "../../io/jsonloader.hpp"
#include <glm/gtx/transform.hpp>

void ProjectileSystem::update(World& world, float delta) {
	// maybe remove ghostcomponent and system.
	auto physWorld = Engine::getInstance().getSystem<BulletPhysicsSystem>()->getWorld();
	int numManifolds = physWorld->getDispatcher()->getNumManifolds();
	for (int i = 0; i < numManifolds; i++) {
		btPersistentManifold* contactManifold = physWorld->getDispatcher()->getManifoldByIndexInternal(i);
		const btCollisionObject* obA = contactManifold->getBody0();
		const btCollisionObject* obB = contactManifold->getBody1();
		auto entityA = static_cast<Entity*>(obA->getUserPointer());
		auto entityB = static_cast<Entity*>(obB->getUserPointer());

		auto projComp = entityA->getComponent<ProjectileComponent>();
		bool isAProj = false;
		if (!projComp) {
			if (!(projComp = entityB->getComponent<ProjectileComponent>()))
				continue;
		} else {
			isAProj = true;
		}

		int numContacts = contactManifold->getNumContacts();
		for (int j = 0; j < numContacts; j++) {
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
			// Loops through everything and take the first object that was collided with.
			// And breaks the for-loop after that.
			if (pt.getDistance() < 0.f && !entityB->isDead()) {
				const glm::vec3& ptA = cast(pt.getPositionWorldOnA());
				// const glm::vec3& ptB = cast(pt.getPositionWorldOnB());
				const glm::vec3& normalOnB = cast(pt.m_normalWorldOnB);
				if (abs(normalOnB.y) > 0)
					continue;

				auto targetLifeComp = entityA->getComponent<LifeComponent>();
				if (targetLifeComp) {
					targetLifeComp->currHP -= projComp->damage;
					targetLifeComp->hpchanged = true;
				}
				auto particleEntity = world.addEntity(sole::uuid4(), "ProjCollisionParticles");
				particleEntity->addComponent<ParticleComponent>()->addEmitter(ptA,
					glm::vec3(0, 1, 0), ParticleComponent::ParticleEffect::EXPLOSION);
				auto particleComp = particleEntity->addComponent<LifeComponent>();
				particleComp->currHP = particleComp->maxHP = 5;
				if (isAProj)
					printf("Apparently it is like thislol\n");
				else {
					auto projComp = entityB->getComponent<ProjectileComponent>();
					if (projComp->bounceCount > 0) {
						auto loader = Engine::getInstance().getJSONLoader();
						const std::string filePath = "assets/entities/player_projectile.json";
						auto newProjectile = loader->constructEntity(world, sole::uuid4(), filePath, json());

						auto newTransComp = newProjectile->getComponent<TransformComponent>();
						auto newRbComp = newProjectile->getComponent<RigidBodyComponent>();
						auto entityBRbComp = entityB->getComponent<RigidBodyComponent>();
						auto entityBTrans = entityB->getComponent<TransformComponent>();
						glm::vec3 reflection = glm::reflect(entityBTrans->getDirection(), glm::vec3(-normalOnB.x, normalOnB.y, normalOnB.z));
						newTransComp->setDirection(glm::vec3(-reflection.x, reflection.y, reflection.z));
						newTransComp->setPosition(ptA + (newTransComp->getDirection() * glm::vec3(0.5f)));
						newTransComp->setScale(entityB->getComponent<TransformComponent>()->getScale());

						newRbComp->setHitboxHalfSize(entityBRbComp->getHitboxHalfSize());
						newRbComp->setTransform(newTransComp);
						newRbComp->getRigidBody()->applyCentralImpulse(cast(newTransComp->getDirection() * projComp->speed));
						newRbComp->setActivationState(DISABLE_DEACTIVATION);
						projComp->bounceCount -= 1;
						newProjectile->getComponent<ProjectileComponent>()->bounceCount = projComp->bounceCount;
						printf("First: %i Second: %i\n", projComp->bounceCount, newProjectile->getComponent<ProjectileComponent>()->bounceCount);
						Engine::getInstance().getSystem<BulletPhysicsSystem>()->addRigidBody(newRbComp,
							BulletPhysicsSystem::CollisionType::COL_PLAYER_PROJECTILE,
							BulletPhysicsSystem::playerProjectileCollidesWith);
					}
					if (projComp->pierceCount > 0 && entityA->getComponent<LifeComponent>()) {
						auto loader = Engine::getInstance().getJSONLoader();
						const std::string filePath = "assets/entities/player_projectile.json";
						auto newProjectile = loader->constructEntity(world, sole::uuid4(), filePath, json());
						auto newTransComp = newProjectile->getComponent<TransformComponent>();
						auto newRbComp = newProjectile->getComponent<RigidBodyComponent>();
						auto entityBRbComp = entityB->getComponent<RigidBodyComponent>();
						auto entityBTrans = entityB->getComponent<TransformComponent>();
						newTransComp->setDirection(glm::vec3(-entityBTrans->getDirection().x, entityBTrans->getDirection().y, entityBTrans->getDirection().z));
						newTransComp->setPosition(ptA + (newTransComp->getDirection()));
						newTransComp->setScale(entityB->getComponent<TransformComponent>()->getScale());

						newRbComp->setHitboxHalfSize(entityBRbComp->getHitboxHalfSize());
						newRbComp->setTransform(newTransComp);
						newRbComp->getRigidBody()->applyCentralImpulse(cast(newTransComp->getDirection() * projComp->speed));
						newRbComp->setActivationState(DISABLE_DEACTIVATION);
						projComp->pierceCount -= 1;
						newProjectile->getComponent<ProjectileComponent>()->pierceCount = projComp->pierceCount;
						Engine::getInstance().getSystem<BulletPhysicsSystem>()->addRigidBody(newRbComp,
							BulletPhysicsSystem::CollisionType::COL_PLAYER_PROJECTILE,
							BulletPhysicsSystem::playerProjectileCollidesWith);
					}
				}
				entityB->makeDead();
			}
		}
	}
}

void ProjectileSystem::registerImGui() {}
