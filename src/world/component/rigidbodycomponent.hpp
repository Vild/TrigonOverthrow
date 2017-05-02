#pragma once
#include "component.hpp"
#include "transformcomponent.hpp"
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <glm/glm.hpp>
#include "../entity.hpp"
class RigidBodyComponent : public Component
{
public:
	RigidBodyComponent(Entity* entity, float mass = 0.0f, float friction = 0.0f);
	virtual ~RigidBodyComponent();

	// Inherited via Component
	virtual std::string name() override;
	virtual void registerImGui() override;

	btRigidBody * getRigidBody();
	btMotionState * getMotionState();
	glm::vec3 getHitboxHalfSize();

	void setMass(btScalar mass);
	void setFriction(btScalar friction);
	void setHitboxHalfSize(const glm::vec3 & size);
	void setTransform(TransformComponent * transform);

	void setActivationState(int newState);

private:
	btRigidBody * rigidBody;

	btScalar friction;
	btScalar mass;
	btMotionState * state;
	btBoxShape * shape;

	glm::vec3 hitboxHalfSize;

};