#include "mainmenustate.hpp"

#include "../world/component/transformcomponent.hpp"
#include "../world/component/cameracomponent.hpp"
#include "../world/component/buttoncomponent.hpp"
#include "../world/component/textcomponent.hpp"

#include "../world/component/lookatcomponent.hpp"
#include "../world/component/particlecomponent.hpp"
#include "../world/component/modelcomponent.hpp"

#include "../engine.hpp"

MainMenuState::MainMenuState() {
	std::shared_ptr<TextFactory> tf = Engine::getInstance().getTextFactory();
	_camera = _world.addEntity(sole::uuid4(), "Camera");
	_title = _world.addEntity(sole::uuid4(), "Title");
	_play = _world.addEntity(sole::uuid4(), "Play");
	_quit = _world.addEntity(sole::uuid4(), "Quit");

	{
		_camera->addComponent<TransformComponent>();
		_camera->addComponent<CameraComponent>();
		auto lookAt = _camera->addComponent<LookAtComponent>();
		lookAt->target = _play;
		lookAt->followMode = FollowMode::followByOffset;
		lookAt->offsetFromTarget = glm::vec3{0, 0, -4};
	}

	{
		/*auto transform = */ _title->addComponent<TransformComponent>();
		auto text = _title->addComponent<TextComponent>();
		text->textRenderer = tf->makeRenderer("Trigon", true, 0);
	}

	{
		/*auto transform = */ _play->addComponent<TransformComponent>();
		/*auto button =*/_play->addComponent<ButtonComponent>();
		auto text = _play->addComponent<TextComponent>();
		text->textRenderer = tf->makeRenderer("Play", true, 0);
	}

	{
		/*auto transform = */ _quit->addComponent<TransformComponent>();
		/*auto button =*/_quit->addComponent<ButtonComponent>();
		auto text = _quit->addComponent<TextComponent>();
		text->textRenderer = tf->makeRenderer("Quit", true, 0);
	}
}

void MainMenuState::onEnter(State* prev) {}
void MainMenuState::onLeave(State* next) {}