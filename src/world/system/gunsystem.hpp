#pragma once
#include "system.hpp"
#include "../component/guncomponent.hpp"
#include "../component/hitboxcomponent.hpp"

class GunSystem : public System {
public:
	virtual void update(World& world, float delta);
	virtual void registerImGui();
	inline virtual std::string name() { return "ParticleSystem"; };

private:
	bool fireRay(std::unique_ptr<Entity>& target, HitboxComponent::HitboxType inType);
	void fireProjectile(Entity* me, Entity* projectile);
};