// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "exporbsystem.hpp"
#include "../component/exporbcomponent.hpp"
#include "../component/transformcomponent.hpp"

#include "../../engine.hpp"

void ExpOrbSystem::update(World& world, float delta) {
	
	Engine& engine = Engine::getInstance();

	for (Entity * entity : Entity::getEntities<ExpOrbComponent>()) {
		auto expOrbComp = entity->getComponent<ExpOrbComponent>();
		if (expOrbComp)
		{
			engine.getState().getPlayer()->getComponent<TransformComponent>()->getPosition();
		}
	}
}

void ExpOrbSystem::registerImGui() {}