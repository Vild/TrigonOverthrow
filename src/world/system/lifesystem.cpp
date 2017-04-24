#include "lifesystem.hpp"
#include "../component/lifecomponent.hpp"

void LifeSystem::update(World& world, float delta) {
	for (std::unique_ptr<Entity>& entity : world.getEntities()) {
		auto lifeComp = entity->getComponent<LifeComponent>();
		if (!lifeComp)
			continue;

		if (lifeComp->currHP <= 0)
			entity->makeDead();
		if(entity->getName() == "Enemy")
			printf("MaxHP: %i, currHP: %i\n", lifeComp->maxHP, lifeComp->currHP);
	}
}

void LifeSystem::registerImGui() {
	
}