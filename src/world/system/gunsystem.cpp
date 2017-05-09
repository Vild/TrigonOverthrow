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
	rmt_ScopedCPUSample(GunSystem, RMTSF_None);
	std::vector<Entity*> toAdd;
	for (std::unique_ptr<Entity>& entity : world.getEntities()) {
		auto currGunComp = entity->getComponent<GunComponent>();
		if (!currGunComp)
			continue;

		if (currGunComp->shoot) {
			toAdd.push_back(entity.get());
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
	auto loader = Engine::getInstance().getJSONLoader();
	const std::string filePath = "assets/entities/player_projectile.json";

	auto transComp = me->getComponent<TransformComponent>();
	auto projectile = loader->constructEntity(world, sole::uuid4(), filePath, json());

	auto transProj = projectile->getComponent<TransformComponent>();
	transProj->setRotation(transComp->getRotation());
	transProj->setScale(glm::vec3(0.075, 0.075, 0.25));
	auto dir = transComp->getDirection();
	transProj->setDirection({-dir.x, dir.y, dir.z}); // helt kokt
	transProj->setPosition(transComp->getPosition() + transProj->getDirection());

	auto point = projectile->getComponent<PointLightComponent>();
	point->pointLight.diffuse = glm::vec3(1, 0, 0);
	point->pointLight.specular = glm::vec3(0.05, 0, 0);
	point->pointLight.constant = 1;
	point->pointLight.linear = 0.35;
	point->pointLight.quadratic = 0.44;

	auto projRdbComp = projectile->getComponent<RigidBodyComponent>();

	projRdbComp->setHitboxHalfSize(transProj->getScale());
	projRdbComp->setMass(1);
	projRdbComp->setFriction(0);
	projRdbComp->getRigidBody()->applyCentralImpulse(cast(transProj->getDirection() * 6.0f));
	projRdbComp->setTransform(transProj);
	projRdbComp->setActivationState(DISABLE_DEACTIVATION);

	auto projLifeComp = projectile->getComponent<LifeComponent>();
	projLifeComp->currHP = projLifeComp->maxHP = 1;

	auto upgradeComp = me->getComponent<UpgradeComponent>();
	if (upgradeComp && upgradeComp->multipleRayMultiplier > 0) {
		/// XXX: because i'm good at programming.
		projectile->makeDead();
		for (int i = -1 * upgradeComp->multipleRayMultiplier; i <= upgradeComp->multipleRayMultiplier; i++) {
			auto newProj = loader->constructEntity(world, sole::uuid4(), filePath, json());
			auto newRbComp = newProj->getComponent<RigidBodyComponent>();
			auto newTrans = newProj->getComponent<TransformComponent>();
			newTrans->setScale(transProj->getScale());
			newTrans->setRotation(transProj->getRotation() * glm::quat_cast(glm::rotate(i * 0.25f, glm::vec3(0, 1, 0))));
			newTrans->setPosition(transComp->getPosition() + newTrans->getDirection());
			newRbComp->setHitboxHalfSize(transProj->getScale());
			newRbComp->setTransform(newTrans);
			newRbComp->getRigidBody()->applyCentralImpulse(cast(newTrans->getDirection() * 6.0f));
			newRbComp->setActivationState(DISABLE_DEACTIVATION);
			Engine::getInstance().getSystem<BulletPhysicsSystem>()->addRigidBody(newRbComp,
				BulletPhysicsSystem::CollisionType::COL_PLAYER_PROJECTILE,
				BulletPhysicsSystem::playerProjectileCollidesWith);
		}
	}
	else if(me->getComponent<GunComponent>()->type == GunComponent::GunType::RAYGUN){
		Engine::getInstance().getSystem<BulletPhysicsSystem>()->addRigidBody(projRdbComp,
			BulletPhysicsSystem::CollisionType::COL_PLAYER_PROJECTILE,
			BulletPhysicsSystem::playerProjectileCollidesWith);
	}
	else {
		Engine::getInstance().getSystem<BulletPhysicsSystem>()->addRigidBody(projRdbComp, 
			BulletPhysicsSystem::CollisionType::COL_ENEMY_PROJECTILE,
			BulletPhysicsSystem::enemyProjectileCollidesWith);
	}
}

void GunSystem::registerImGui() {}
