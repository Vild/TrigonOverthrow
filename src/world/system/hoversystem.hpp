#pragma once
#include "system.hpp"

class HoverSystem : public System {
public:
	HoverSystem();
	virtual ~HoverSystem();

	// Inherited via System
	void update(World& world, float delta) final;
	void registerImGui() final;
	inline std::string name() final { return "HoverSystem"; };

private:
};
