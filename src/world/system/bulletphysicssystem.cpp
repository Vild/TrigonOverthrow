#include "bulletphysicssystem.hpp"

BulletPhyisicsSystem::BulletPhyisicsSystem()
{
	collisionConfig = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfig);
	constraintSolver = new btSequentialImpulseConstraintSolver();
	broadphaseInterface = new btDbvtBroadphase();
	
	world = new btDiscreteDynamicsWorld(dispatcher, broadphaseInterface, constraintSolver, collisionConfig);
	world->setGravity({ 0, -9.82f, 0 });
}

BulletPhyisicsSystem::~BulletPhyisicsSystem()
{
	delete world;
	delete dispatcher;
	delete collisionConfig;
	delete constraintSolver;
	delete broadphaseInterface;
}

void BulletPhyisicsSystem::update(World & w, float delta)
{
	world->stepSimulation(delta);
}

void BulletPhyisicsSystem::registerImGui()
{
}

std::string BulletPhyisicsSystem::name()
{
	return "BulletPhyisicsSystem";
}
