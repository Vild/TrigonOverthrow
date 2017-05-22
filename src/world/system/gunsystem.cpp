// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "gunsystem.hpp"
#include "../component/transformcomponent.hpp"
#include "../component/pointlightcomponent.hpp"
#include "../component/guncomponent.hpp"
#include "../component/lifecomponent.hpp"
#include "../component/modelcomponent.hpp"
#include "../component/projectilecomponent.hpp"
#include "../component/instancedsimplemeshcomponent.hpp"
#include "../component/dynamicmodelcomponent.hpp"
#include "../component/upgradecomponent.hpp"
#include "../../io/jsonloader.hpp"

#include "../system/bulletphysicssystem.hpp"
#include <glm/gtx/transform.hpp>

#include "../entity.hpp"
#include "../../gl/mesh.hpp"
#include "../../io/meshloader.hpp"
#include "../../engine.hpp"

GunSystem::~GunSystem() {}

void GunSystem::update(World& world, float delta) {
	if (delta == 0)
		return;
	std::vector<Entity*> toAdd;
	for (Entity* entity : world.getActiveComponents<GunComponent>()) {
		auto currGunComp = entity->getComponent<GunComponent>();

		if (currGunComp->shoot) {
			toAdd.push_back(entity);
			currGunComp->shoot = false;
			currGunComp->cooldown = currGunComp->cooldownLength;
		}
		if (currGunComp->cooldown > 0)
			currGunComp->cooldown -= 1 * delta;
	}
	for (Entity* entity : toAdd) {
		_fireProjectile(entity, world);
	}
}

void GunSystem::_fireProjectile(Entity* me, World& world) {
	auto type = me->getComponent<GunComponent>()->type;
	switch (type)
	{
	case GunComponent::RAYGUN: {
		auto loader = Engine::getInstance().getJSONLoader();
		const std::string filePath = "assets/entities/player_projectile.json";

		auto transComp = me->getComponent<TransformComponent>();
		auto projectile = loader->constructEntity(world, sole::uuid4(), filePath, json());

		auto transProj = projectile->getComponent<TransformComponent>();
		transProj->setRotation(transComp->getRotation());
		transProj->setScale(glm::vec3(0.075, 0.075, 0.25));
		auto dir = transComp->getDirection();
		transProj->setDirection({ -dir.x, dir.y, dir.z }); // helt kokt
		transProj->setPosition(transComp->getPosition() + transProj->getDirection());

		auto projRdbComp = projectile->getComponent<RigidBodyComponent>();
		auto projComp = projectile->getComponent<ProjectileComponent>();

		projRdbComp->setMass(1);
		projRdbComp->setFriction(0);
		projRdbComp->getRigidBody()->applyCentralImpulse(cast(transProj->getDirection() * projComp->speed));
		projRdbComp->setTransform(transProj);
		projRdbComp->setActivationState(DISABLE_DEACTIVATION);


		auto upgradeComp = me->getComponent<UpgradeComponent>();
		projComp->bounceCount = upgradeComp->reflectionCount;
		projComp->pierceCount = upgradeComp->refractionCount;
		if (upgradeComp && upgradeComp->multipleRayMultiplier > 0) {
			/// XXX: because i'm good at programming.
			projectile->makeDead();
			for (int i = -1 * upgradeComp->multipleRayMultiplier; i <= upgradeComp->multipleRayMultiplier; i++) {
				auto newProj = loader->constructEntity(world, sole::uuid4(), filePath, json());
				auto newRbComp = newProj->getComponent<RigidBodyComponent>();
				auto newTrans = newProj->getComponent<TransformComponent>();
				auto newProjComp = newProj->getComponent<ProjectileComponent>();
				newProjComp->bounceCount = upgradeComp->reflectionCount;
				newProjComp->pierceCount = upgradeComp->refractionCount;
				newTrans->setScale(transProj->getScale());
				newTrans->setRotation(transProj->getRotation() * glm::quat_cast(glm::rotate(i * 0.25f, glm::vec3(0, 1, 0))));
				newTrans->setPosition(transComp->getPosition() + newTrans->getDirection());
				newRbComp->setHitboxHalfSize(transProj->getScale());
				newRbComp->setTransform(newTrans);
				newRbComp->getRigidBody()->applyCentralImpulse(cast(newTrans->getDirection() * projComp->speed));
				newRbComp->setActivationState(DISABLE_DEACTIVATION);
				Engine::getInstance().getSystem<BulletPhysicsSystem>()->addRigidBody(newRbComp, BulletPhysicsSystem::CollisionType::COL_PLAYER_PROJECTILE,
					BulletPhysicsSystem::playerProjectileCollidesWith);
			}
		}
		Engine::getInstance().getSystem<BulletPhysicsSystem>()->addRigidBody(projRdbComp, BulletPhysicsSystem::CollisionType::COL_PLAYER_PROJECTILE,
			BulletPhysicsSystem::playerProjectileCollidesWith);
	}
		break;
	case GunComponent::ENERGYGUN: {
		auto loader = Engine::getInstance().getJSONLoader();
		auto player = Engine::getInstance().getState().getPlayer();
		auto playerPos = player->getComponent<TransformComponent>()->getPosition();
		const std::string filePath = "assets/entities/enemy_projectile.json";

		auto transComp = me->getComponent<TransformComponent>();
		glm::vec3 dir = normalize(playerPos - transComp->getPosition());
		auto projectile = loader->constructEntity(world, sole::uuid4(), filePath, json());

		auto transProj = projectile->getComponent<TransformComponent>();
		transProj->setRotation(transComp->getRotation());
		transProj->setScale(glm::vec3(0.1));
		transProj->setDirection({ -dir.x, dir.y, dir.z }); // helt kokt
		transProj->setPosition(transComp->getPosition() + dir);

		auto projRdbComp = projectile->getComponent<RigidBodyComponent>();
		auto projComp = projectile->getComponent<ProjectileComponent>();

		projRdbComp->setMass(1);
		projRdbComp->setFriction(0);
		projRdbComp->getRigidBody()->applyCentralImpulse(cast(transProj->getDirection() * projComp->speed));
		projRdbComp->setTransform(transProj);
		projRdbComp->setActivationState(DISABLE_DEACTIVATION);

		auto projLifeComp = projectile->getComponent<LifeComponent>();
		projLifeComp->currHP = projLifeComp->maxHP = 3;

		Engine::getInstance().getSystem<BulletPhysicsSystem>()->addRigidBody(projRdbComp, BulletPhysicsSystem::CollisionType::COL_ENEMY_PROJECTILE,
			BulletPhysicsSystem::enemyProjectileCollidesWith);
	}
		break;
	default:
		break;
	}
}

void GunSystem::registerImGui() {}
