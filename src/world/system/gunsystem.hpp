#pragma once
#include "system.hpp"
#include "../component/guncomponent.hpp"
#include "../component/hitboxcomponent.hpp"
class GunSystem : public System {
public:
	virtual ~GunSystem();

	void testAgainst(std::unique_ptr<Entity>& target, HitboxComponent::HitboxType inType, std::shared_ptr<GunComponent::Gun> gun);

	virtual void update(World& world, float delta);
	virtual void registerImGui();
	inline virtual std::string name() { return "ParticleSystem"; };

private:
};