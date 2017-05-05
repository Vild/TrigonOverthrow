// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "ghostobjectcomponent.hpp"

GhostObjectComponent::GhostObjectComponent() {
	hitboxHalfSize = {1, 1, 1};
	_friction = 0.0f;
	_shape = new btBoxShape({1, 1, 1});
	_ghostObject = new btPairCachingGhostObject();
	_ghostObject->setActivationState(DISABLE_DEACTIVATION);
}

GhostObjectComponent::~GhostObjectComponent() {
	delete _shape;
	delete _ghostObject;
}

void GhostObjectComponent::registerImGui() {}

btPairCachingGhostObject* GhostObjectComponent::getGhostObject() {
	return _ghostObject;
}

glm::vec3 GhostObjectComponent::getHitboxHalfSize() {
	return hitboxHalfSize;
}

void GhostObjectComponent::setFriction(btScalar friction) {
	_friction = friction;
	_ghostObject->setFriction(friction);
}

void GhostObjectComponent::setHitboxHalfSize(const glm::vec3& size) {
	hitboxHalfSize = size;
	delete _shape;
	_shape = new btBoxShape({size.x, size.y, size.z});
	_ghostObject->setCollisionShape(_shape);
}
