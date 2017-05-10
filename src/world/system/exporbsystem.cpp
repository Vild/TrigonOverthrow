// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "exporbsystem.hpp"
#include "../component/exporbcomponent.hpp"

void ExpOrbSystem::update(World& world, float delta) {
	for (std::unique_ptr<Entity>& entity : world.getEntities()) {
		auto expOrbComp = entity->getComponent<ExpOrbComponent>();
		if (expOrbComp)
		{
			//world.addEntity(sole::uuid4(), "ExpOrb");
		}
	}
}

void ExpOrbSystem::registerImGui() {}