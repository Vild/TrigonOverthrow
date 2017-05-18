#pragma once
#include "component.hpp"

struct AIComponent : public Component {
	enum ProjectilePattern : int {straight = 0};
	enum MovePattern : int {normal = 0, sinus, circle};

	AIComponent() = default;
	AIComponent(const ComponentValues& value);
	virtual ~AIComponent();
	inline std::string name() final { return "AIComponent"; }
	void registerImGui() final;

	ProjectilePattern attackType;
	MovePattern moveType;
	float usefulTimer;
	bool doInverse;
};