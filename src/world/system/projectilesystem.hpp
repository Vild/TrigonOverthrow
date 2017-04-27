#pragma once
#include "system.hpp"
#include <BulletCollision\CollisionDispatch\btCollisionDispatcher.h>

class ProjectileSystem : public System {
public:
	ProjectileSystem() {}
	virtual ~ProjectileSystem() {}
	virtual void update(World& world, float delta);
	virtual void registerImGui();
	inline virtual std::string name() { return "ProjectileSystem"; }
private:

};