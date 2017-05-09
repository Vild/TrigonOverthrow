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

#include "../system/bulletphysicssystem.hpp"

#include "../entity.hpp"
#include "../../gl/mesh.hpp"
#include "../../io/meshloader.hpp"
#include "../../engine.hpp"

GunSystem::~GunSystem() {}

void GunSystem::update(World& world, float delta) {
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
		fireProjectile(entity, world.addEntity(sole::uuid4(), "Projectile"));
	}
}

void GunSystem::fireProjectile(Entity* me, Entity* projectile) {
	auto transComp = me->getComponent<TransformComponent>();
	auto transProj = projectile->addComponent<TransformComponent>();

	transProj->setRotation(transComp->getRotation());
	transProj->setScale(glm::vec3(0.075, 0.075, 0.25));
	auto dir = transComp->getDirection();
	transProj->setDirection({-dir.x, dir.y, dir.z}); // helt kokt
	transProj->setPosition(transComp->getPosition() + transProj->getDirection());

	auto point = projectile->addComponent<PointLightComponent>();
	point->pointLight.diffuse = glm::vec3(1, 0, 0);
	point->pointLight.specular = glm::vec3(0.05, 0, 0);
	point->pointLight.constant = 1;
	point->pointLight.linear = 0.35;
	point->pointLight.quadratic = 0.44;

	// auto currRdbComp = me->getComponent<RigidBodyComponent>();
	auto projRdbComp = projectile->addComponent<RigidBodyComponent>(projectile);

	projRdbComp->setHitboxHalfSize(transProj->getScale());
	projRdbComp->setMass(1);
	projRdbComp->setFriction(0);
	projRdbComp->getRigidBody()->applyCentralImpulse(cast(transProj->getDirection() * 6.0f));
	projRdbComp->setTransform(transProj);
	projRdbComp->setActivationState(DISABLE_DEACTIVATION);

	auto projLifeComp = projectile->addComponent<LifeComponent>();
	projLifeComp->currHP = projLifeComp->maxHP = 1;

	/*auto projComp =*/projectile->addComponent<ProjectileComponent>(1.0f);

	auto modelComp = projectile->addComponent<ModelComponent>();
	modelComp->meshData = Engine::getInstance().getMeshLoader()->getMesh("assets/objects/player_projectile.fbx");
	modelComp->meshData->texture = Engine::getInstance().getTextureManager()->getTexture("assets/textures/errorNormal.png");
	modelComp->meshData->mesh
		->addBuffer("m",
								[](GLuint id) {
									glBindBuffer(GL_ARRAY_BUFFER, id);
									glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);

									for (int i = 0; i < 4; i++) {
										glEnableVertexAttribArray(ShaderAttributeID::m + i);
										glVertexAttribPointer(ShaderAttributeID::m + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4) * i));
										glVertexAttribDivisor(ShaderAttributeID::m + i, 1);
									}

									glBindBuffer(GL_ARRAY_BUFFER, 0);
								})
		.finalize();

	if (me->getComponent<GunComponent>()->type == GunComponent::GunType::RAYGUN)
		Engine::getInstance().getSystem<BulletPhysicsSystem>()->addRigidBody(projRdbComp, BulletPhysicsSystem::CollisionType::COL_PLAYER_PROJECTILE,
																																				 BulletPhysicsSystem::playerProjectileCollidesWith);
	else
		Engine::getInstance().getSystem<BulletPhysicsSystem>()->addRigidBody(projRdbComp, BulletPhysicsSystem::CollisionType::COL_ENEMY_PROJECTILE,
																																				 BulletPhysicsSystem::playerProjectileCollidesWith);
}

void GunSystem::registerImGui() {}
