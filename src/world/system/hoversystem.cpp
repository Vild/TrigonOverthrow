// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "hoversystem.hpp"
#include "../component/hovercomponent.hpp"
#include "../component/transformcomponent.hpp"
#include "../component/rigidbodycomponent.hpp"
#include "bulletphysicssystem.hpp"

HoverSystem::HoverSystem() {}

HoverSystem::~HoverSystem() {}

void HoverSystem::update(World& world, float delta) {
	btDynamicsWorld* btWorld = BulletPhysicsSystem::activeInstance->getWorld();

	for (Entity * e : Entity::getEntities<HoverComponent>()) {
		HoverComponent* hc;
		TransformComponent* tc;
		RigidBodyComponent* rbc;

		if ((hc = e->getComponent<HoverComponent>()) && (tc = e->getComponent<TransformComponent>()) && (rbc = e->getComponent<RigidBodyComponent>())) {
			glm::vec3 pos = tc->getPosition();
			btVector3 rayStart(pos.x, pos.y, pos.z);
			btVector3 rayMax(pos.x, pos.y - hc->hoverHeight * 2, pos.z);

			btCollisionWorld::ClosestRayResultCallback rayCallback(rayStart, rayMax);
			// rayCallback.m_collisionFilterGroup = btBroadphaseProxy::DefaultFilter;
			// rayCallback.m_collisionFilterMask = btBroadphaseProxy::AllFilter;
			rayCallback.m_collisionFilterGroup = BulletPhysicsSystem::COL_PLAYER;
			rayCallback.m_collisionFilterMask = BulletPhysicsSystem::COL_WALL;
			btWorld->rayTest(rayStart, rayMax, rayCallback);

			if (rayCallback.hasHit()) {
				float distance = rayCallback.m_closestHitFraction;

				float hoverFactor = (hc->hoverHeight - distance) / hc->hoverHeight;
				btVector3 force = btVector3(0, 1, 0) * hoverFactor * hc->hoverForce * rbc->getMass();
				rbc->getRigidBody()->applyCentralForce(force);
			}
		}
	}
}

void HoverSystem::registerImGui() {}
