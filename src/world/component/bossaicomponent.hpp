#pragma once
#include "component.hpp"
#include "../entity.hpp"
#include <vector>

struct BossAIComponent : public Component {
	enum BossStates : int {monologue = 0, firstPhase, secondPhase, finalPhase};
	BossAIComponent() = default;
	BossAIComponent(const ComponentValues& value);
	virtual ~BossAIComponent();
	inline std::string name() final { return "BossAIComponent"; }
	void registerImGui() final;

	BossStates currState;
	float usefulTimer;
	bool doInverse;

	float monologueTimer;
	int currentBatch;
	std::vector<Entity*> projectiles;
};
