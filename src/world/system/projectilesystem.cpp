// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "projectilesystem.hpp"
#include "bulletphysicssystem.hpp"
#include "../component/projectilecomponent.hpp"
#include "../component/particlecomponent.hpp"
#include "../component/lifecomponent.hpp"
#include "../../engine.hpp"

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
		LifeComponent* targetLifeComp;
		bool isAProj = false;
		if (!projComp) {
			if (!(projComp = entityB->getComponent<ProjectileComponent>()))
				continue;
			else
				targetLifeComp = entityA->getComponent<LifeComponent>();
		} else {
			targetLifeComp = entityB->getComponent<LifeComponent>();
			isAProj = true;
		}

		int numContacts = contactManifold->getNumContacts();
		for (int j = 0; j < numContacts; j++) {
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
			// Loops through everything and take the first object that was collided with.
			// And breaks the for-loop after that.
			if (pt.getDistance() < 0.f) {
				const btVector3& ptA = pt.getPositionWorldOnA();
				const btVector3& ptB = pt.getPositionWorldOnB();
				//const btVector3& normalOnB = pt.m_normalWorldOnB;
				if (isAProj) {
					if (targetLifeComp) {
						targetLifeComp->currHP -= projComp->damage;
						targetLifeComp->hpchanged = true;
						auto particleComp = world.addEntity(sole::uuid4(), "ProjCollisionParticles")->addComponent<ParticleComponent>();
						particleComp->addEmitter(cast(ptB),
							glm::vec3(0, 1, 0), ParticleComponent::ParticleEffect::INITIATE);
					}
					entityA->makeDead();
				} else {
					if (targetLifeComp) {
						targetLifeComp->currHP -= projComp->damage;
						targetLifeComp->hpchanged = true;
						auto particleComp = world.addEntity(sole::uuid4(), "ProjCollisionParticles")->addComponent<ParticleComponent>();
						particleComp->addEmitter(cast(ptA),
							glm::vec3(0, 1, 0), ParticleComponent::ParticleEffect::INITIATE);
					}
					entityB->makeDead();
				}
				break;
			}
		}
		break;
	}
}

void ProjectileSystem::registerImGui() {}
