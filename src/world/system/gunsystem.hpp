#pragma once
#include "system.hpp"
#include "../component/guncomponent.hpp"

class GunSystem : public System {
public:
	virtual ~GunSystem();
	void update(World& world, float delta) final;
	void registerImGui() final;
	inline std::string name() final { return "GunSystem"; };

private:
	void _fireProjectile(Entity* me, World& world);
	void _bossFireProjectile(Entity* boss, World& world);
};
