#pragma once
#include "system.hpp"

class ProjectileSystem : public System {
public:
	virtual ~ProjectileSystem() {}
	virtual void update(World& world, float delta);
	virtual void registerImGui();
	inline virtual std::string name() { return "ProjectileSystem"; }

private:

};