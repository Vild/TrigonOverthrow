#include "hitboxsystem.hpp"
#include "../entity.hpp"
#include "../src/world/component/hitboxcomponent.hpp"

void HitboxSystem::update(World& world, float delta) {
	for (std::unique_ptr<Entity>& entity : world.getEntities()) {
		auto hitbox = entity->getComponent<HitboxComponent>();
		//if(!hitbox)
	}
}

void HitboxSystem::registerImGui() {
	
}
