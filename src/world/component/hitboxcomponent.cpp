// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "hitboxcomponent.hpp"

HitboxComponent::HitboxComponent(const ComponentValues& value) : HitboxComponent() {
	// TODO:
}

HitboxComponent::~HitboxComponent() {}

void HitboxComponent::addHitbox(HitboxComponent::HitboxType type, glm::vec3 pos) {
	switch (type) {
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

void HitboxComponent::registerImGui() {}
