#include "experiencesystem.hpp"
#include "../component/experiencecomponent.hpp"
#include "../component/upgradecomponent.hpp"

ExperienceSystem::~ExperienceSystem() {
	
}

void ExperienceSystem::update(World& world, float delta) {
	for (std::unique_ptr<Entity>& entity: world.getEntities()) {
		auto expComp = entity->getComponent<ExperienceComponent>();
		if (!expComp)
			continue;
		if (expComp->currExp >= expComp->expToNextLevel) {
			entity->getComponent<UpgradeComponent>()->upgradePoints += 1;
			expComp->currExp -= expComp->expToNextLevel;
			expComp->expToNextLevel += 5;
		}
	}
}

void ExperienceSystem::registerImGui() {
	
}