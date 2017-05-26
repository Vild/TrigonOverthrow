#pragma once
#include "component.hpp"
#include "../entity.hpp"
#include <vector>

struct BossAIComponent : public Component {
	enum BossStates : int {monologue = 0, firstPhase, finalPhase};
	BossAIComponent() = default;
	BossAIComponent(const ComponentValues& value);
	virtual ~BossAIComponent();
	inline std::string name() final { return "BossAIComponent"; }
	void registerImGui() final;

	BossStates currState;
	float usefulTimer;
	glm::vec3 dir;

	float monologueTimer;
	int currentBatch;
	std::vector<Entity*> projectiles;
};
