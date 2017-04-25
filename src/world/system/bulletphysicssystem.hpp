#pragma once
#include "system.hpp"
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <vector>
#include "../component/rigidbodycomponent.hpp"

class BulletPhyisicsSystem : public System
{
public:
	BulletPhyisicsSystem();
	virtual ~BulletPhyisicsSystem();
	// Inherited via System
	virtual void update(World & world, float delta) override;
	virtual void registerImGui() override;
	virtual std::string name() override;

	void addRigidBody(RigidBodyComponent *  rigidBody);

private:
	btDynamicsWorld * world;
	btDispatcher * dispatcher;
	btCollisionConfiguration * collisionConfig;
	btBroadphaseInterface * broadphaseInterface;
	btConstraintSolver * constraintSolver;

	std::vector<RigidBodyComponent*> bodies;

	std::unique_ptr<btRigidBody> planeBody;
	std::unique_ptr<btMotionState> planeState;
	std::unique_ptr<btStaticPlaneShape> planeShape;
};