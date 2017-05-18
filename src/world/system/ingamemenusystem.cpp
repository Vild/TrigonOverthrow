#include "ingamemenusystem.hpp"

#include "../component/ingamemenucomponent.hpp"
#include "../../engine.hpp"
#include "../../io/hidinput.hpp"

InGameMenuSystem::~InGameMenuSystem() {}

void InGameMenuSystem::update(World& world, float delta) {
	Engine& engine = Engine::getInstance();
	State& state = engine.getState();
	auto hidInput = engine.getHIDInput();

	if (escapePressed && updateUI)
		updateUI(state, showUI = !showUI);

	escapePressed = false;

	for (Entity* entity : world.getActiveComponents<InGameMenuComponent>()) {
		InGameMenuComponent* menu = entity->getComponent<InGameMenuComponent>();
		if (menu->renderUI)
			menu->renderUI(*entity, state);
	}
}

void InGameMenuSystem::registerImGui() {}