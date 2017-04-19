#include "mainmenustate.hpp"

#include "../world/component/transformcomponent.hpp"
#include "../world/component/cameracomponent.hpp"
#include "../world/component/particlecomponent.hpp"
#include "../world/component/textcomponent.hpp"
#include "../world/component/modelcomponent.hpp"

MainMenuState::MainMenuState() {
	_camera = _world.addEntity(sole::uuid4(), "Camera");
	_title = _world.addEntity(sole::uuid4(), "Title");
	_play = _world.addEntity(sole::uuid4(), "Play");
	_quit = _world.addEntity(sole::uuid4(), "Quit");

	{
		_camera->addComponent<TransformComponent>();
		_camera->addComponent<CameraComponent>();
	}

	{ auto transform = _title->addComponent<TransformComponent>(); }

	{ auto transform = _play->addComponent<TransformComponent>(); }

	{ auto transform = _quit->addComponent<TransformComponent>(); }
}

void MainMenuState::onEnter(State* prev) {}
void MainMenuState::onLeave(State* next) {}