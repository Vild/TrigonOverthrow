// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "ghostobjectsystem.hpp"
#include "../component/ghostobjectcomponent.hpp"
#include "bulletphysicssystem.hpp"
#include "../../engine.hpp"
GhostObjectSystem::GhostObjectSystem() {}

GhostObjectSystem::~GhostObjectSystem() {}

void GhostObjectSystem::update(World& world, float delta) {
	for (Entity * entity : Entity::getEntities<GhostObjectComponent>()) {
		auto ghostObjComp = entity->getComponent<GhostObjectComponent>();
		if (!ghostObjComp)
			continue;
	}
}

void GhostObjectSystem::registerImGui() {}
