#pragma once
#include "system.hpp"
#include "../component/projectilecomponent.hpp"
#include <BulletCollision/CollisionDispatch/btCollisionDispatcher.h>

class ProjectileSystem : public System {
public:
	ProjectileSystem() {}
	virtual ~ProjectileSystem() {}
	void update(World& world, float delta) final;
	void registerImGui() final;
	inline std::string name() final { return "ProjectileSystem"; }

private:
	void _createPiercingProjectile(World& world, ProjectileComponent* prevProj, Entity* self, const glm::vec3& collPoint, const glm::vec3& otherNormal);
	void _createBouncingProjectile(World& world, ProjectileComponent* prevProj, Entity* self, const glm::vec3& collPoint, const glm::vec3& otherNormal);
};
