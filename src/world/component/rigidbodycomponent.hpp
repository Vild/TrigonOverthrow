#pragma once
#include "component.hpp"
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

class RigidBodyComponent : public Component<RigidBodyComponent>
{
public:
	RigidBodyComponent(btScalar mass, btMotionState * state, btCollisionShape * shape);
	virtual ~RigidBodyComponent();

	// Inherited via Component
	virtual std::string name() override;
	virtual void registerImGui() override;

private:
	btRigidBody * rigidBody;
};