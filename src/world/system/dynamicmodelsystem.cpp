// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "dynamicmodelsystem.hpp"
#include "../component/dynamicmodelcomponent.hpp"
#include "../component/lifecomponent.hpp"

DynamicModelSystem::DynamicModelSystem() {}

void DynamicModelSystem::update(World& world, float delta) {
	for (Entity * entity : world.getActiveComponents<DynamicModelComponent>()) {
		auto dynamicModelComp = entity->getComponent<DynamicModelComponent>();
		auto lifeComp = entity->getComponent<LifeComponent>();

		if (!lifeComp)
			continue;

		auto modelComp = entity->getComponent<ModelComponent>();
		if (lifeComp->hpchanged && lifeComp->currHP > 0) {
			lifeComp->hpchanged = false;
			modelComp->meshData = dynamicModelComp->meshes[lifeComp->currHP - 1];
		}
	}
}

void DynamicModelSystem::registerImGui() {}