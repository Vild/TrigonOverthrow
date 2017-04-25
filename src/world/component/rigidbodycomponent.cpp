#include "rigidbodycomponent.hpp"
#include <glm/gtc/type_ptr.hpp>

RigidBodyComponent::RigidBodyComponent()
{
	//btStaticPlaneShape * plane = new btStaticPlaneShape({ 0,1,0 }, 0);
	hitboxHalfSize = { 1,1,1 };

	mass = 0.0f;
	friction = 0.0f;
	state = new btDefaultMotionState();
	shape = new btBoxShape({ 1,1,1 });

	rigidBody = new btRigidBody(mass, state, shape);
	rigidBody->setActivationState(DISABLE_DEACTIVATION);
}

RigidBodyComponent::~RigidBodyComponent()
{
	delete state;
	delete shape;
	delete rigidBody;
}

std::string RigidBodyComponent::name()
{
	return "RigidBodyComponent";
}

void RigidBodyComponent::registerImGui()
{
	if (ImGui::DragFloat("Mass", &mass))
		setMass(mass);

	if (ImGui::DragFloat("Friction", &friction))
		setFriction(friction);

	if (ImGui::DragFloat3("Hitbox Size", glm::value_ptr(hitboxHalfSize))) 
		setHitboxHalfSize(hitboxHalfSize);
}

btRigidBody * RigidBodyComponent::getRigidBody()
{
	return rigidBody;
}

btMotionState * RigidBodyComponent::getMotionState()
{
	return state;
}

glm::vec3 RigidBodyComponent::getHitboxHalfSize()
{
	return hitboxHalfSize;
}

void RigidBodyComponent::setMass(btScalar mass)
{
	this->mass = mass;
	rigidBody->setMassProps(mass, rigidBody->getLocalInertia());
}

void RigidBodyComponent::setFriction(btScalar friction)
{
	this->friction = friction;
	rigidBody->setFriction(friction);
}

void RigidBodyComponent::setHitboxHalfSize(const glm::vec3 & size)
{
	hitboxHalfSize = size;
	delete shape; 
	shape = new btBoxShape({ size.x, size.y, size.z });
	rigidBody->setCollisionShape(shape);
}

void RigidBodyComponent::setTransform(TransformComponent * transform)
{
	state->setWorldTransform(btTransform(
		cast(transform->getRotation()),
		cast(transform->getPosition())
	));
}
