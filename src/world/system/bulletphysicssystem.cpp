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
	world->setGravity({ 0, -10.f, 0 });
}

BulletPhyisicsSystem::~BulletPhyisicsSystem()
{
	delete world;
	delete broadphaseInterface;
	delete constraintSolver;
	delete dispatcher;
	delete collisionConfig;
}

void BulletPhyisicsSystem::update(World & w, float delta)
{
	world->stepSimulation(delta);

	for (std::shared_ptr<Entity> entity : w.getEntities()) 
	{
		auto rigidbody = entity->getComponent<RigidBodyComponent>();
		if (!rigidbody) continue;

		auto transform = entity->getComponent<TransformComponent>();
		if (!transform) continue;

		btMotionState * state = rigidbody->getMotionState();
		btTransform t; state->getWorldTransform(t);
		
		btQuaternion btQ = t.getRotation();
		glm::quat q = {btQ.getW(), btQ.getX(), btQ.getY(), btQ.getZ()};

		btScalar m[16]; t.getOpenGLMatrix(m);
		glm::vec3 pos = {m[3], m[13], m[23]};

		transform->setPosition(pos);
		transform->setRotation(q);
	}
}

void BulletPhyisicsSystem::registerImGui()
{
}

std::string BulletPhyisicsSystem::name()
{
	return "BulletPhyisicsSystem";
}

void BulletPhyisicsSystem::addRigidBody(std::shared_ptr<RigidBodyComponent> rigidBody)
{
	world->addRigidBody(rigidBody->getRigidBody());
}
