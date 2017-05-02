#include "ghostobjectsystem.hpp"
#include "../component/ghostobjectcomponent.hpp"
#include "bulletphysicssystem.hpp"
#include "../../engine.hpp"
GhostObjectSystem::GhostObjectSystem() {
	
}

GhostObjectSystem::~GhostObjectSystem() {
	
}

void GhostObjectSystem::update(World & world, float delta){
	for (std::unique_ptr<Entity>& entity : world.getEntities()) {
		auto ghostObjComp = entity->getComponent<GhostObjectComponent>();
		if (!ghostObjComp)
			continue;
	}
}

void GhostObjectSystem::registerImGui(){

}

