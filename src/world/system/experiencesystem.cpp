#include "experiencesystem.hpp"
#include "../component/experiencecomponent.hpp"
#include "../component/upgradecomponent.hpp"
#include "../component/experienceorbcomponent.hpp"
#include "../component/transformcomponent.hpp"
#include "../component/rigidbodycomponent.hpp"
#include "../../engine.hpp"

ExperienceSystem::~ExperienceSystem() {

}

void ExperienceSystem::update(World& world, float delta) {
	auto player = Engine::getInstance().getState().getPlayer();
	if (!player)
		return;

	auto playerEXP = player->getComponent<ExperienceComponent>();
	for (std::unique_ptr<Entity>& entity: world.getEntities()) {
		auto expORB = entity->getComponent<ExperienceOrbComponent>();
		if (!expORB)
			continue;

		auto orbPos = entity->getComponent<TransformComponent>()->getPosition();
		auto playerPos = player->getComponent<TransformComponent>()->getPosition();
		float distance = glm::distance(playerPos, orbPos);
		if ((distance <= playerEXP->pickUpRadius) || expORB->hasBeenPicked) {
			if (distance <= 0.3f) {
				entity->makeDead();
				playerEXP->currExp += expORB->amntOfExp;
			}
			expORB->hasBeenPicked = true;
			auto rdbComp = entity->getComponent<RigidBodyComponent>()->getRigidBody();
			rdbComp->setLinearVelocity(cast(glm::normalize(playerPos - orbPos) * glm::vec3(4)));
		}
	}
	if(playerEXP)
		while (playerEXP->currExp >= playerEXP->expToNextLevel) {
			player->getComponent<UpgradeComponent>()->upgradePoints += 1;
			playerEXP->currExp -= playerEXP->expToNextLevel;
			playerEXP->expToNextLevel += 5;
		}
}

void ExperienceSystem::registerImGui() {

}