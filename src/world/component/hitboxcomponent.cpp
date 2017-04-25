#include "hitboxcomponent.hpp"

void HitboxComponent::addHitbox(HitboxComponent::HitboxType type, glm::vec3 pos) {
	switch (type)
	{
	case HitboxComponent::SPHERE: {
		this->type = type;
		std::shared_ptr<HitboxSphere> tempSphere = std::make_shared<HitboxSphere>();
		tempSphere->center = pos;
		tempSphere->radius2 = 0.8 * 0.8f;
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

void HitboxComponent::registerImGui() {
	
}