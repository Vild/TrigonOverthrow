#include "dynamicmodelsystem.hpp"
#include "../component/dynamicmodelcomponent.hpp"
#include "../component/lifecomponent.hpp"

DynamicModelSystem::DynamicModelSystem() {
	
}

void DynamicModelSystem::update(World& world, float delta) {
	for (std::unique_ptr<Entity>& entity : world.getEntities()) {
		auto dynamicModelComp = entity->getComponent<DynamicModelComponent>();
		if (!dynamicModelComp)
			continue;

		auto modelComp = entity->getComponent<ModelComponent>();
		auto lifeComp = entity->getComponent<LifeComponent>();
		if (lifeComp->hpchanged && lifeComp->currHP > 0) {
			lifeComp->hpchanged = false;
			modelComp->meshData = dynamicModelComp->meshes[lifeComp->currHP];
		}
	}
}

void DynamicModelSystem::registerImGui() {
	
}