// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "bulletphysicssystem.hpp"
#include "../component/rigidbodycomponent.hpp"
#include "../component/transformcomponent.hpp"
#include "../component/projectilecomponent.hpp"
#include "../component/experienceorbcomponent.hpp"
#include <Bullet3Common/b3Transform.h>
#include <glm/gtc/type_ptr.hpp>
#include "../../engine.hpp"

BulletPhysicsSystem* BulletPhysicsSystem::activeInstance = nullptr;

BulletPhysicsSystem::BulletPhysicsSystem() {
	activeInstance = this;

	collisionConfig = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfig);
	constraintSolver = new btSequentialImpulseConstraintSolver();
	broadphaseInterface = new btDbvtBroadphase();

	world = new btDiscreteDynamicsWorld(dispatcher, broadphaseInterface, constraintSolver, collisionConfig);
}

BulletPhysicsSystem::~BulletPhysicsSystem() {
	delete world;
	delete broadphaseInterface;
	delete constraintSolver;
	delete dispatcher;
	delete collisionConfig;
}

void BulletPhysicsSystem::update(World& w, float delta) {
	world->stepSimulation(delta);
	for (Entity * entity : w.getActiveComponents<RigidBodyComponent>()) {
		auto rigidbody = entity->getComponent<RigidBodyComponent>();
		if (!rigidbody)
			continue;

		auto transform = entity->getComponent<TransformComponent>();
		if (!transform)
			continue;

		auto projComp = entity->getComponent<ProjectileComponent>();
		if (projComp || entity->getComponent<ExperienceOrbComponent>())
			rigidbody->getRigidBody()->setGravity(btVector3(0, 0, 0));

		btTransform t = rigidbody->getRigidBody()->getWorldTransform();

		btVector3 o = t.getOrigin();
		btQuaternion q = t.getRotation();

		transform->setPosition(cast(o));
		// transform->setRotation(cast(q));
	}
}

void BulletPhysicsSystem::registerImGui() {}

void BulletPhysicsSystem::addRigidBody(RigidBodyComponent* rigidBody, int group, int mask) {
	world->addRigidBody(rigidBody->getRigidBody(), group, mask);
}

void BulletPhysicsSystem::removeRigidBody(RigidBodyComponent* rigidBody) {
	world->removeRigidBody(rigidBody->getRigidBody());
}
