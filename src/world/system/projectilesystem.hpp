#pragma once
#include "system.hpp"
#include <BulletCollision/CollisionDispatch/btCollisionDispatcher.h>

class ProjectileSystem : public System {
public:
	ProjectileSystem() {}
	virtual ~ProjectileSystem() {}
	void update(World& world, float delta) final;
	void registerImGui() final;
	inline std::string name() final { return "ProjectileSystem"; }

private:
};
