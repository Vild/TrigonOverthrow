// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "mainmenustate.hpp"

#include <glm/gtx/transform.hpp>

#include "../world/component/transformcomponent.hpp"
#include "../world/component/cameracomponent.hpp"
#include "../world/component/buttoncomponent.hpp"
#include "../world/component/textcomponent.hpp"
#include "../world/component/lookatcomponent.hpp"
#include "../world/component/modelcomponent.hpp"

#include "../engine.hpp"
#include "../state/ingamestate.hpp"

MainMenuState::MainMenuState() {
	std::shared_ptr<TextFactory> tf = Engine::getInstance().getTextFactory();
	_camera = _world.addEntity(sole::uuid4(), "Camera");
	_target = _world.addEntity(sole::uuid4(), "Target");
	_title = _world.addEntity(sole::uuid4(), "Title");
	{
		/*auto transform = */ _camera->addComponent<TransformComponent>();
		// transform->recalculateMatrix();
		_camera->addComponent<CameraComponent>();

		auto lookAt = _camera->addComponent<LookAtComponent>();
		lookAt->target = _target;
		lookAt->followMode = FollowMode::followByOffset;
		lookAt->offsetFromTarget = glm::vec3{0, 0, -4};
	}

	{
		/*auto transform = */ _target->addComponent<TransformComponent>();
		//	transform->recalculateMatrix();
	}

	{
		auto transform = _title->addComponent<TransformComponent>();
		transform->setPosition({0, 0.4, 0});
		// transform->recalculateMatrix();

		auto text = _title->addComponent<TextComponent>();
		text->textRenderer = tf->makeRenderer("Trigon", false, 10);
		text->transform.setPosition({-0.3, 0, 0});
		text->transform.setScale({5, 5, 5});
	}
}

MainMenuState::~MainMenuState() {}

void MainMenuState::onEnter(State* prev) {}

void MainMenuState::onLeave(State* next) {}

void MainMenuState::registerImGui() {
	Engine& engine = Engine::getInstance();
	ImGuiWindowFlags windowFlags =
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse;
	ImGui::Begin("MainMenu", nullptr, windowFlags);
	if (ImGui::Button("Play"))
		engine.setState<InGameState>();

	if (ImGui::Button("Quit"))
		engine.quit();
	ImGui::End();
}