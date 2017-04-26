#pragma once
#include "component.hpp"
#include "transformcomponent.hpp"
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <glm/glm.hpp>

class RigidBodyComponent : public Component
{
public:
	RigidBodyComponent();
	virtual ~RigidBodyComponent();

	// Inherited via Component
	virtual std::string name() override;
	virtual void registerImGui() override;

	btRigidBody * getRigidBody();
	btMotionState * getMotionState();
	glm::vec3 getHitboxHalfSize();
	inline btScalar getMass() { return mass; }

	void setMass(btScalar mass);
	void setFriction(btScalar friction);
	void setHitboxHalfSize(const glm::vec3 & size);
	void setTransform(TransformComponent * transform);

private:
	btRigidBody * rigidBody;

	btScalar friction;
	btScalar mass;
	btMotionState * state;
	btBoxShape * shape;

	glm::vec3 hitboxHalfSize;

};