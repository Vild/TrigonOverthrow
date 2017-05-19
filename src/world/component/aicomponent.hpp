#pragma once
#include "component.hpp"

struct AIComponent : public Component {
	enum MovePattern : int {normal = 0, sinus, circle};

	AIComponent() = default;
	AIComponent(const ComponentValues& value);
	virtual ~AIComponent();
	inline std::string name() final { return "AIComponent"; }
	void registerImGui() final;

	MovePattern moveType;
	float usefulTimer;
	bool doInverse;
};