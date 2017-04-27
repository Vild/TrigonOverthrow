#pragma once
#include "system.hpp"
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <vector>
#include "../component/rigidbodycomponent.hpp"
#define BIT(x) (1<<(x))

class BulletPhysicsSystem : public System
{
public:
	enum CollisionType {
		COL_NOTHING = 0,
		COL_WALL = BIT(1),
		COL_PLAYER = BIT(2),
		COL_ENEMY = BIT(3),
		COL_PROJECTILE = BIT(4)
	};

	const static int playerCollidesWith = COL_WALL | COL_ENEMY | COL_PROJECTILE;
	const static int enemyCollidesWith = COL_WALL | COL_PLAYER | COL_PROJECTILE;
	const static int projectileCollidesWith = COL_PLAYER | COL_ENEMY;
	const static int wallCollidesWith = COL_PLAYER | COL_ENEMY;

	BulletPhysicsSystem();
	virtual ~BulletPhysicsSystem();

	// Inherited via System
	virtual void update(World & world, float delta) override;
	virtual void registerImGui() override;
	virtual std::string name() override;

	void addRigidBody(RigidBodyComponent *  rigidBody, int group, int mask);
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