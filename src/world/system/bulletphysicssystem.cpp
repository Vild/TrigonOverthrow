#include "bulletphysicssystem.hpp"
#include "../component/rigidbodycomponent.hpp"
#include "../component/transformcomponent.hpp"
#include "../component/projectilecomponent.hpp"
#include <Bullet3Common/b3Transform.h>
#include <glm/gtc/type_ptr.hpp>
#include "../../engine.hpp"

BulletPhysicsSystem::BulletPhysicsSystem()
{
	collisionConfig = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfig);
	constraintSolver = new btSequentialImpulseConstraintSolver();
	broadphaseInterface = new btDbvtBroadphase();

	world = new btDiscreteDynamicsWorld(dispatcher, broadphaseInterface, constraintSolver, collisionConfig);
	world->setGravity({ 0, -9.82f, 0 });
}

BulletPhysicsSystem::~BulletPhysicsSystem()
{
	//auto & objs = world->getCollisionObjectArray();
	//for (int i = 0; i < objs.size(); i++)
	//{
	//	world->removeCollisionObject(objs[i]);
	//}

	//for (int i = world->getNumCollisionObjects() - 1; i >= 0; i--)
	//{
	//	btCollisionObject* obj = world->getCollisionObjectArray()[i];
	//	//btRigidBody* body = btRigidBody::upcast(obj);
	//	//if (body && body->getMotionState())
	//	//{
	//	//	delete body->getMotionState();
	//	//}
	//	world->removeCollisionObject(obj);
	//	//delete obj;
	//}


	delete world;
	delete broadphaseInterface;
	delete constraintSolver;
	delete dispatcher;
	delete collisionConfig;
}

void BulletPhysicsSystem::update(World & w, float delta)
{
	rmt_ScopedCPUSample(BulletPhyisicsSystem, RMTSF_None);
	world->stepSimulation(delta);

	for (std::unique_ptr<Entity>& entity : w.getEntities())
	{
		auto rigidbody = entity->getComponent<RigidBodyComponent>();
		if (!rigidbody) continue;

		auto transform = entity->getComponent<TransformComponent>();
		if (!transform) continue;

		auto projComp = entity->getComponent<ProjectileComponent>();
		if (projComp)
			rigidbody->getRigidBody()->setGravity(btVector3(0, 0, 0));


		btTransform t = rigidbody->getRigidBody()->getWorldTransform();


		btVector3 o = t.getOrigin();
		btQuaternion q = t.getRotation();

		transform->setPosition(cast(o));
		transform->setRotation(cast(q));
	}
}

void BulletPhysicsSystem::registerImGui()
{
}

std::string BulletPhysicsSystem::name()
{
	return "BulletPhyisicsSystem";
}

void BulletPhysicsSystem::addRigidBody(RigidBodyComponent * rigidBody, int group, int mask)
{
	world->addRigidBody(rigidBody->getRigidBody(), group, mask);
}

void BulletPhysicsSystem::removeRigidBody(RigidBodyComponent * rigidBody)
{
	world->removeRigidBody(rigidBody->getRigidBody());
}
