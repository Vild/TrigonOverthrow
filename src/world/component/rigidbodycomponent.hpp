#pragma once
#include "component.hpp"
#include "transformcomponent.hpp"
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <glm/glm.hpp>
#include "../entity.hpp"
class RigidBodyComponent : public Component {
public:
	RigidBodyComponent() = default;
	RigidBodyComponent(Entity* entity, float mass = 0.0f, float friction = 0.0f, float damp = 0.0f);
	RigidBodyComponent(const ComponentValues& value);
	virtual ~RigidBodyComponent();

	// Inherited via Component
	inline std::string name() final { return "RigidBodyComponent"; }
	void registerImGui() final;

	btRigidBody* getRigidBody();
	btMotionState* getMotionState();
	glm::vec3 getHitboxHalfSize();

	void setMass(btScalar mass);
	void setFriction(btScalar friction);
	void setHitboxHalfSize(const glm::vec3& size);
	void setTransform(TransformComponent* transform);

	void setActivationState(int newState);

	inline float getMass() { return mass; }

private:
	btRigidBody* rigidBody;

	btScalar friction;
	btScalar mass;
	btMotionState* state;
	btBoxShape* shape;

	glm::vec3 hitboxHalfSize;
};
