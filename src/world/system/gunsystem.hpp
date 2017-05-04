#pragma once
#include "system.hpp"
#include "../component/guncomponent.hpp"
#include "../component/hitboxcomponent.hpp"

class GunSystem : public System {
public:
	virtual ~GunSystem();
	void update(World& world, float delta) final;
	void registerImGui() final;
	inline std::string name() final { return "GunSystem"; };

private:
	// bool fireRay(std::unique_ptr<Entity>& target, HitboxComponent::HitboxType inType);
	void fireProjectile(Entity* me, Entity* projectile);
};
