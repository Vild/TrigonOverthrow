// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "lifesystem.hpp"
#include "../component/lifecomponent.hpp"

void LifeSystem::update(World& world, float delta) {
	for (Entity * entity : Entity::getEntities<LifeComponent>()) {
		auto lifeComp = entity->getComponent<LifeComponent>();
		if (!lifeComp)
			continue;

		if (lifeComp->currHP <= 0)
			entity->makeDead();

		if (entity->getName() == "Projectile") {
			lifeComp->currHP -= 1 * delta;
		}
	}
}

void LifeSystem::registerImGui() {}