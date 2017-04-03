#pragma once

#include "component.hpp"

#include "../entity/entity.hpp"
#include <memory>

struct LookAtComponent : public Component<LookAtComponent> {
	std::shared_ptr<Entity> target;
	float minDistance = 1.0f;
	float maxDistance = 2.0f;

	virtual void registerImGui();
	virtual std::string name() { return "LookAtComponent"; }
};
