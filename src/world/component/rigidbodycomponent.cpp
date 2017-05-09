// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "rigidbodycomponent.hpp"
#include <glm/gtc/type_ptr.hpp>

RigidBodyComponent::RigidBodyComponent(Entity* entity, float mass, float friction, float damping) {
	// btStaticPlaneShape * plane = new btStaticPlaneShape({ 0,1,0 }, 0);
	hitboxHalfSize = {1, 1, 1};

	this->mass = mass;
	this->friction = friction;
	state = new btDefaultMotionState();
	shape = new btBoxShape({1, 1, 1});
	rigidBody = new btRigidBody(mass, state, shape);
	rigidBody->setUserPointer(entity);
	rigidBody->setDamping(damping, 0);
}

RigidBodyComponent::RigidBodyComponent(const ComponentValues& value) : RigidBodyComponent() {
	hitboxHalfSize = {1, 1, 1};

	mass = value.getFloat("mass", 1);
	friction = value.getFloat("friction", 1);
	state = new btDefaultMotionState();
	shape = new btBoxShape({1, 1, 1});
	rigidBody = new btRigidBody(mass, state, shape);
	rigidBody->setUserPointer(value.entity);
	rigidBody->setDamping(value.getFloat("damping", 1), 0);
}

RigidBodyComponent::~RigidBodyComponent() {
	delete state;
	delete shape;
	delete rigidBody;
}

void RigidBodyComponent::registerImGui() {
	if (ImGui::DragFloat("Mass", &mass))
		setMass(mass);

	if (ImGui::DragFloat("Friction", &friction))
		setFriction(friction);

	if (ImGui::DragFloat3("Hitbox Size", glm::value_ptr(hitboxHalfSize)))
		setHitboxHalfSize(hitboxHalfSize);
}

btRigidBody* RigidBodyComponent::getRigidBody() {
	return rigidBody;
}

btMotionState* RigidBodyComponent::getMotionState() {
	return state;
}

glm::vec3 RigidBodyComponent::getHitboxHalfSize() {
	return hitboxHalfSize;
}

void RigidBodyComponent::setMass(btScalar mass) {
	this->mass = mass;
	rigidBody->setMassProps(mass, rigidBody->getLocalInertia());
}

void RigidBodyComponent::setFriction(btScalar friction) {
	this->friction = friction;
	rigidBody->setFriction(friction);
}

void RigidBodyComponent::setHitboxHalfSize(const glm::vec3& size) {
	hitboxHalfSize = size;
	delete shape;
	shape = new btBoxShape({size.x, size.y, size.z});
	rigidBody->setCollisionShape(shape);
}

void RigidBodyComponent::setTransform(TransformComponent* transform) {
	rigidBody->setWorldTransform(btTransform(cast(transform->getRotation()), cast(transform->getPosition())));
}

void RigidBodyComponent::setActivationState(int newState) {
	rigidBody->setActivationState(newState);
}
