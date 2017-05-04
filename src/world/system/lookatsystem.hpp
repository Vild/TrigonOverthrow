#pragma once

#include "system.hpp"

class LookAtSystem : public System {
public:
	virtual ~LookAtSystem();

	void update(World& world, float delta) final;

	void registerImGui() final;
	inline std::string name() final { return "LookAtSystem"; };
};
