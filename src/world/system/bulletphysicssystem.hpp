#pragma once
#include "system.hpp"
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <vector>
#include "../component/rigidbodycomponent.hpp"

class BulletPhysicsSystem : public System
{
public:
	BulletPhysicsSystem();
	virtual ~BulletPhysicsSystem();

	// Inherited via System
	virtual void update(World & world, float delta) override;
	virtual void registerImGui() override;
	virtual std::string name() override;

	void addRigidBody(RigidBodyComponent *  rigidBody);
	void removeRigidBody(RigidBodyComponent *  rigidBody);
	btDynamicsWorld* getWorld() { return world; }

private:
	btDynamicsWorld * world;
	btDispatcher * dispatcher;
	btCollisionConfiguration * collisionConfig;
	btBroadphaseInterface * broadphaseInterface;
	btConstraintSolver * constraintSolver;

	std::vector<btRigidBody*> bodies;

	std::unique_ptr<btRigidBody> planeBody;
	std::unique_ptr<btMotionState> planeState;
	std::unique_ptr<btStaticPlaneShape> planeShape;
};