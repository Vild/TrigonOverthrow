// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "mainmenustate.hpp"

#include <glm/gtx/transform.hpp>

#include "../world/component/transformcomponent.hpp"
#include "../world/component/cameracomponent.hpp"
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
		_camera->addComponent<TransformComponent>();
		_camera->addComponent<CameraComponent>();

		auto lookAt = _camera->addComponent<LookAtComponent>();
		lookAt->target = _target;
		lookAt->followMode = FollowMode::followByOffset;
		lookAt->offsetFromTarget = glm::vec3{0, 0, -4};
	}

	{ _target->addComponent<TransformComponent>(); }

	{
		auto transform = _title->addComponent<TransformComponent>();
		transform->setPosition({0, 0.4, 0});

		auto text = _title->addComponent<TextComponent>();
		text->textRenderer = tf->makeRenderer("Trigon", false, 10);
		text->transform.setPosition({-0.35, 0, 0});
		text->transform.setScale({5, 5, 5});
	}
}

MainMenuState::~MainMenuState() {}

void MainMenuState::onEnter(State* prev) {}

void MainMenuState::onLeave(State* next) {}

void MainMenuState::registerImGui() {
	Engine& engine = Engine::getInstance();
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
																 ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse;
	ImGui::PushFont(engine.getBigFont());
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1, 0, 1, 1));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1, 0, 0, 1));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 1, 0, 1));

	ImGui::SetNextWindowPosCenter(ImGuiSetCond_Always);
	float scale = 1.0 - (512.0 / engine.getWidth());
	ImGui::Begin("MainMenu", nullptr, windowFlags);
	ImGui::SetWindowFontScale(scale);
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 64);
		ImGui::Dummy(ImVec2(0, (128 + 64) * scale));
		if (ImGui::Button("Play", ImVec2((256 * 2 + 64) * scale, (128 + 16) * scale)))
			engine.setState<InGameState>();

		ImGui::Dummy(ImVec2(0, 64 * scale));
		if (ImGui::Button("Quit", ImVec2((256 * 2 + 64) * scale, (128 + 16) * scale)))
			engine.quit();
		ImGui::PopStyleVar();
	}
	ImGui::End();

	ImGui::PopStyleColor(4);
	ImGui::PopFont();
}
