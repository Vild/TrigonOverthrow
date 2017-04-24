#include "hitboxcomponent.hpp"

HitboxComponent::~HitboxComponent() {}

void HitboxComponent::addHitbox(HitboxComponent::HitboxType type, glm::vec3 pos) {
	switch (type) {
	case HitboxComponent::SPHERE: {
		this->type = type;
		std::shared_ptr<HitboxSphere> tempSphere = std::make_shared<HitboxSphere>();
		tempSphere->center = pos;
		tempSphere->radius2 = 1.0f * 1.0f;
		hitbox = tempSphere;
		break;
	}
	case HitboxComponent::TETRAHEDRON: {
		break;
	}
	case HitboxComponent::AABB: {
		break;
	}
	default:
		break;
	}
}

void HitboxComponent::registerImGui() {}