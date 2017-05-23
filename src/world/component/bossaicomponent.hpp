#pragma once
#include "component.hpp"

struct BossAIComponent : public Component {
	BossAIComponent() = default;
	BossAIComponent(const ComponentValues& value);
	virtual ~BossAIComponent();
	inline std::string name() final { return "BossAIComponent"; }
	void registerImGui() final;

	float usefulTimer;
	bool doInverse;
};
