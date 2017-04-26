#include "bulletphysicssystem.hpp"
#include "../component/rigidbodycomponent.hpp"
#include "../component/transformcomponent.hpp"
#include <Bullet3Common\b3Transform.h>
#include <glm\gtc\type_ptr.hpp>

BulletPhyisicsSystem::BulletPhyisicsSystem()
{
	collisionConfig = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfig);
	constraintSolver = new btSequentialImpulseConstraintSolver();
	broadphaseInterface = new btDbvtBroadphase();
	
	world = new btDiscreteDynamicsWorld(dispatcher, broadphaseInterface, constraintSolver, collisionConfig);
	world->setGravity({ 0, -9.82f, 0 });

	planeShape = std::make_unique<btStaticPlaneShape>(btVector3(0, 1, 0), 0);
	planeState = std::make_unique<btDefaultMotionState>();
	planeBody = std::make_unique<btRigidBody>(0, planeState.get(), planeShape.get());

	//world->addRigidBody(planeBody.get());
}

BulletPhyisicsSystem::~BulletPhyisicsSystem()
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

void BulletPhyisicsSystem::update(World & w, float delta)
{
	world->stepSimulation(delta);

	for (std::unique_ptr<Entity>& entity : w.getEntities()) 
	{
		auto rigidbody = entity->getComponent<RigidBodyComponent>();
		if (!rigidbody) continue;

		auto transform = entity->getComponent<TransformComponent>();
		if (!transform) continue;

		btMotionState * state = rigidbody->getMotionState();
		btTransform t; state->getWorldTransform(t);

		btVector3 o = t.getOrigin();
		btQuaternion q = t.getRotation();

		transform->setPosition(cast(o));
		transform->setRotation(cast(q));
	}
}

void BulletPhyisicsSystem::registerImGui()
{
}

std::string BulletPhyisicsSystem::name()
{
	return "BulletPhyisicsSystem";
}

void BulletPhyisicsSystem::addRigidBody(RigidBodyComponent * rigidBody)
{
	world->addRigidBody(rigidBody->getRigidBody());
}

void BulletPhyisicsSystem::removeRigidBody(RigidBodyComponent * rigidBody)
{
	world->removeRigidBody(rigidBody->getRigidBody());
}
