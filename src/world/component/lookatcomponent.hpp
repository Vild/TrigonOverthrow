#pragma once

#include "component.hpp"

#include "../entity.hpp"
#include <memory>
#include <glm/glm.hpp>

enum class FollowMode : int {
	followByDistance, /// This mode will have a calculated rotation
	followByOffset		/// This mode will follow the target at a specified offset. This means that the rotation will always be constant
};

struct LookAtComponent : public Component {
	Entity* target;

	FollowMode followMode = FollowMode::followByDistance;

	// for FollowMode::followByDistance
	float minDistance = 1.0f;
	float maxDistance = 2.0f;

	// for FollowMode::followByOffset
	glm::vec3 offsetFromTarget;

	virtual void registerImGui();
	virtual std::string name() { return "LookAtComponent"; }
};
