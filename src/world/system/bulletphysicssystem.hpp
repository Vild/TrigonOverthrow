#pragma once
#include "system.hpp"
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <vector>
#include "../component/rigidbodycomponent.hpp"
#define BIT(x) (1 << (x))

class BulletPhysicsSystem : public System {
public:
	static BulletPhysicsSystem* activeInstance;

	enum CollisionType {
		COL_NOTHING = 0,
		COL_WALL = BIT(0),
		COL_PLAYER = BIT(1),
		COL_ENEMY = BIT(2),
		COL_ENEMY_PROJECTILE = BIT(3),
		COL_PLAYER_PROJECTILE = BIT(4)
	};

	const static int playerCollidesWith = COL_WALL | COL_ENEMY | COL_ENEMY_PROJECTILE;
	const static int enemyCollidesWith = COL_WALL | COL_PLAYER | COL_PLAYER_PROJECTILE;
	const static int enemyProjectileCollidesWith = COL_PLAYER | COL_WALL;
	const static int playerProjectileCollidesWith = COL_ENEMY | COL_WALL;
	const static int wallCollidesWith = COL_PLAYER | COL_ENEMY | COL_ENEMY_PROJECTILE | COL_PLAYER_PROJECTILE;

	BulletPhysicsSystem();
	virtual ~BulletPhysicsSystem();

	// Inherited via System
	void update(World& world, float delta) final;
	void registerImGui() final;
	inline std::string name() final { return "BulletPhyisicsSystem"; }

	void addRigidBody(RigidBodyComponent* rigidBody, int group, int mask);
	void removeRigidBody(RigidBodyComponent* rigidBody);
	btDynamicsWorld* getWorld() { return world; }

private:
	btDynamicsWorld* world;
	btDispatcher* dispatcher;
	btCollisionConfiguration* collisionConfig;
	btBroadphaseInterface* broadphaseInterface;
	btConstraintSolver* constraintSolver;

	std::vector<btRigidBody*> bodies;

	std::unique_ptr<btRigidBody> planeBody;
	std::unique_ptr<btMotionState> planeState;
	std::unique_ptr<btStaticPlaneShape> planeShape;
};
