#include "gunsystem.hpp"
#include "../component/transformcomponent.hpp"
#include "../component/guncomponent.hpp"

void GunSystem::update(World& world, float delta) {
	for (std::unique_ptr<Entity>& entity : world.getEntities()) {
		auto gunComp = entity->getComponent<GunComponent>();
		if (!gunComp)
			continue;

		auto transform = entity->getComponent<TransformComponent>();
		if (!transform)
			continue;

		if (entity->getName() == "Player") {
			std::shared_ptr<GunComponent::RayGun> raygun = std::static_pointer_cast<GunComponent::RayGun>(gunComp->gun);
			raygun->ray.o = transform->position;
			//raygun->ray.dir = transform->getDirection();
			raygun->ray.dir = glm::vec3(0, 0, 1);
			if (gunComp->shoot) {
				for (std::unique_ptr<Entity>& target : world.getEntities()) {
					if (target == entity)
						continue;
					
					auto hitbox = target->getComponent<HitboxComponent>();
					if (!hitbox)
						continue;

					testAgainst(target, hitbox->type, gunComp->gun);
				}
				gunComp->shoot = false;
			}
		}
	}
}

void GunSystem::testAgainst(std::unique_ptr<Entity>& target, HitboxComponent::HitboxType inType, std::shared_ptr<GunComponent::Gun> gun) {
	switch (inType)
	{
	case HitboxComponent::SPHERE: {
		float t0, t1;
		std::shared_ptr<HitboxComponent::HitboxSphere> hitbox = std::static_pointer_cast<HitboxComponent::HitboxSphere>(target->getComponent<HitboxComponent>()->hitbox);
		std::shared_ptr<GunComponent::RayGun> raygun = std::static_pointer_cast<GunComponent::RayGun>(gun);
		glm::vec3 L = hitbox->center - raygun->ray.o;
		float tca = glm::dot(L, raygun->ray.dir);
		if (tca < 0)
			return;
		float d2 = glm::dot(L, L) - tca * tca;
		if (d2 > hitbox->radius2)
			return;
		float thc = sqrt(hitbox->radius2 - d2);
		t0 = tca - thc;
		t1 = tca + thc;
		if (t0 > t1)
			std::swap(t0, t1);

		if (t0 < 0) {
			t0 = t1;
			if (t0 < 0)
				return;
		}

		raygun->ray.t[0] = raygun->ray.o + t0 * raygun->ray.dir;
		raygun->ray.t[1] = raygun->ray.o + t1 * raygun->ray.dir;
		gun = raygun;
		printf("Entry: %f %f %f\n, Exit: %f %f %f\n\n", raygun->ray.t[0].x, raygun->ray.t[0].y, raygun->ray.t[0].z,
			raygun->ray.t[1].x, raygun->ray.t[1].y, raygun->ray.t[1].z);
		break;
	}
	case HitboxComponent::TETRAHEDRON: {

		break;
	}
	case HitboxComponent::AABB: {

		break;
	}
	default:
		break;
	}
}


void GunSystem::registerImGui() {
	
}