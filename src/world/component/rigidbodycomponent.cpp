#include "rigidbodycomponent.hpp"

RigidBodyComponent::RigidBodyComponent(btScalar mass, btMotionState * state, btCollisionShape * shape)
{
	//btStaticPlaneShape * plane = new btStaticPlaneShape({ 0,1,0 }, 0);
	//btMotionState * motionState = new btDefaultMotionState();
	rigidBody = new btRigidBody(mass, state, shape);
}

RigidBodyComponent::~RigidBodyComponent()
{
	delete rigidBody;
}

std::string RigidBodyComponent::name()
{
	return "RigidBodyComponent";
}

void RigidBodyComponent::registerImGui()
{
}
