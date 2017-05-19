#pragma once
#include "system.hpp"
#include "../component/aicomponent.hpp"

class AISystem : public System {
public:
	void update(World& world, float delta) final;
	void registerImGui() final;
	inline std::string name() final { return "AISystem"; }
private:
	glm::vec3 _calculateForceDirection(glm::vec3 dir, float time, AIComponent::MovePattern type);
};