#include "gunsystem.hpp"
#include "../component/transformcomponent.hpp"
#include "../component/guncomponent.hpp"
#include "../component/lifecomponent.hpp"
#include "../component/physicscomponent.hpp"
#include "../component/modelcomponent.hpp"
#include "../entity.hpp"
#include "../src/gl/mesh.hpp"
#include "../src/engine.hpp"

void GunSystem::update(World& world, float delta) {
	std::vector<Entity*> toAdd;
	for (std::unique_ptr<Entity>& entity : world.getEntities()) {
		auto currGunComp = entity->getComponent<GunComponent>();
		if (!currGunComp)
			continue;

		if (currGunComp->shoot) {
			toAdd.push_back(entity.get());
			currGunComp->shoot = false;
			currGunComp->cooldown = currGunComp->cooldownLength;
		}
		if(currGunComp->cooldown > 0)
			currGunComp->cooldown -= 1 * delta;
	}
	for (Entity* entity : toAdd) {
		fireProjectile(entity, world.addEntity(sole::uuid4(), "Projectile"));
	}
}

void GunSystem::fireProjectile(Entity* me, Entity* projectile) {
	auto currTransform = me->getComponent<TransformComponent>();
	
	auto projTransComp = projectile->addComponent<TransformComponent>();
	projTransComp->position = currTransform->position;
	projTransComp->rotation = currTransform->rotation;
	projTransComp->scale = glm::vec3(0.3f);
	projTransComp->recalculateMatrix();
	
	auto projPhysComp = projectile->addComponent<PhysicsComponent>();
	projPhysComp->acceleration = glm::vec3(0,0,1);
	projPhysComp->velocity = projPhysComp->acceleration * glm::vec3(2);
	
	auto projLifeComp = projectile->addComponent<LifeComponent>();
	projLifeComp->currHP = projLifeComp->maxHP = 2;

	auto modelComp = projectile->addComponent<ModelComponent>();
	modelComp->meshData = Engine::getInstance().getMeshLoader()->getMesh("assets/objects/player.fbx");
	modelComp->meshData->mesh
		->addBuffer("m",
			[](GLuint id) {
		glBindBuffer(GL_ARRAY_BUFFER, id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);

		for (int i = 0; i < 4; i++) {
			glEnableVertexAttribArray(ShaderAttributeID::m + i);
			glVertexAttribPointer(ShaderAttributeID::m + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4) * i));
			glVertexAttribDivisor(ShaderAttributeID::m + i, 1);
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	})
		.finalize();
}


//physicsComponent->acceleration = -physicsComponent->velocity / 0.05f;
//physicsComponent->acceleration += inputDir.z * forward * accelSpeed;
//physicsComponent->acceleration += inputDir.x * right * accelSpeed;
//physicsComponent->acceleration += inputDir.y * up * accelSpeed;


//bool GunSystem::fireRay(std::unique_ptr<Entity>& target, HitboxComponent::HitboxType inType) {
//	bool hit = false;
//	switch (inType)
//	{
//	case HitboxComponent::SPHERE: {
//		float t0, t1;
//		std::shared_ptr<HitboxComponent::HitboxSphere> hitbox = std::static_pointer_cast<HitboxComponent::HitboxSphere>(target->getComponent<HitboxComponent>()->hitbox);
//		std::shared_ptr<GunComponent::RayGun> raygun = std::static_pointer_cast<GunComponent::RayGun>(gun);
//		glm::vec3 L = hitbox->center - raygun->ray.o;
//		float tca = glm::dot(L, raygun->ray.dir);
//		if (tca < 0)
//			return false;
//		float d2 = glm::dot(L, L) - tca * tca;
//		if (d2 > hitbox->radius2)
//			return false;
//		float thc = sqrt(hitbox->radius2 - d2);
//		t0 = tca - thc;
//		t1 = tca + thc;
//		if (t0 > t1)
//			std::swap(t0, t1);
//
//		if (t0 < 0) {
//			t0 = t1;
//			if (t0 < 0)
//				return false;
//		}
//
//		raygun->ray.t[0] = raygun->ray.o + t0 * raygun->ray.dir;
//		raygun->ray.t[1] = raygun->ray.o + t1 * raygun->ray.dir;
//		gun = raygun;
//		printf("Entry: %f %f %f\n, Exit: %f %f %f\n\n", raygun->ray.t[0].x, raygun->ray.t[0].y, raygun->ray.t[0].z,
//			raygun->ray.t[1].x, raygun->ray.t[1].y, raygun->ray.t[1].z);
//		hit = true;
//		break;
//	}
//	case HitboxComponent::TETRAHEDRON: {
//
//		break;
//	}
//	case HitboxComponent::AABB: {
//
//		break;
//	}
//	default:
//		break;
//	}
//	return hit;
//}

void GunSystem::registerImGui() {
	
}