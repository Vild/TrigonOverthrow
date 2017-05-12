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
	_play = _world.addEntity(sole::uuid4(), "Play");
	_quit = _world.addEntity(sole::uuid4(), "Quit");

	{
		//_camera->addComponent<TransformComponent>()->recalculateMatrix();
		_camera->addComponent<CameraComponent>();

		auto lookAt = _camera->addComponent<LookAtComponent>();
		lookAt->target = _target;
		lookAt->followMode = FollowMode::followByOffset;
		lookAt->offsetFromTarget = glm::vec3{0, 0, -4};
	}

	//{ _target->addComponent<TransformComponent>()->recalculateMatrix(); }

	{
		auto transform = _title->addComponent<TransformComponent>();
		transform->setPosition({0, 0.4, 0});
		// transform->recalculateMatrix();

		auto text = _title->addComponent<TextComponent>();
		text->textRenderer = tf->makeRenderer("Trigon", false, 10);
		text->transform.setPosition({-0.3, 0, 0});
		text->transform.setScale({5, 5, 5});
	}

	{
		auto transform = _play->addComponent<TransformComponent>();
		// transform->position = glm::vec3{0, -0.2, 0};
		transform->setPosition({-0.5625, -0.5625 - 0.2, 0});
		transform->setScale({1.125, 3 * 1.0 / 16, 3 * 1.0 / 16});
		// transform->recalculateMatrix();

		auto button = _play->addComponent<ButtonComponent>();
		button->size = glm::vec3{2, 2, 2};
		button->position = button->size * glm::vec3(-0.5);
		button->callback = &MainMenuState::_onButtonCallback;

		auto text = _play->addComponent<TextComponent>();
		text->textRenderer = tf->makeRenderer(" Play ", false, 6);
		text->transform.setPosition({0, 0, 0});
		text->transform.setScale(glm::vec3{3, 3, 3} / transform->getScale());

		_play->registerImGui = &MainMenuState::_registerImGUI;
		_toggleBox(*_play);
	}

	{
		auto transform = _quit->addComponent<TransformComponent>();
		// transform->position = glm::vec3{0, -0.75, 0};
		transform->setPosition({-0.5625, -0.5625 - 0.75, 0});
		transform->setScale({1.125, 1.0 / 16, 1.0 / 16});
		// transform->recalculateMatrix();

		auto button = _quit->addComponent<ButtonComponent>();
		button->size = glm::vec3{2, 2, 2};
		button->position = button->size * glm::vec3(-0.5);
		button->callback = &MainMenuState::_onButtonCallback;

		auto text = _quit->addComponent<TextComponent>();
		text->textRenderer = tf->makeRenderer(" Quit ", false, 6);
		text->transform.setPosition({0, 0, 0});
		text->transform.setScale(glm::vec3{3, 3, 3} / transform->getScale());

		_quit->registerImGui = &MainMenuState::_registerImGUI;
		_toggleBox(*_quit);
	}
}

MainMenuState::~MainMenuState() {}

void MainMenuState::onEnter(State* prev) {}

void MainMenuState::onLeave(State* next) {}

void MainMenuState::_toggleBox(Entity& entity) {
	if (entity.getComponent<ModelComponent>())
		return entity.removeComponent<ModelComponent>();

	auto model = entity.addComponent<ModelComponent>();

	model->meshData = Engine::getInstance().getMeshLoader()->getMesh("assets/objects/box.obj");
	model->meshData->texture = Engine::getInstance().getTextureManager()->getTexture("assets/textures/errorNormal.png");
	model->meshData->mesh
		->addBuffer("m",
								[](GLuint id) {
									glBindBuffer(GL_ARRAY_BUFFER, id);
									glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);

									for (int i = 0; i < 4; i++) {
										glEnableVertexAttribArray(ShaderAttributeID::m + i);
										glVertexAttribPointer(ShaderAttributeID::m + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4) * i));
										glVertexAttribDivisor(ShaderAttributeID::m + i, 1);
									}

									glBindBuffer(GL_ARRAY_BUFFER, 0);
								})
		.finalize();
}

void MainMenuState::_registerImGUI(Entity& self, State& state) {
	bool doesRender = !!self.getComponent<ModelComponent>();
	if (ImGui::Checkbox("Render box", &doesRender))
		static_cast<MainMenuState&>(state)._toggleBox(self);
}

void MainMenuState::_onButtonCallback(Entity* entity, State& state, MouseState mouse) {
	MainMenuState& self = static_cast<MainMenuState&>(state);
	Engine& engine = Engine::getInstance();
	if (mouse & MouseState::left) {
		if (entity == self._play)
			engine.setState<InGameState>();
		else if (entity == self._quit)
			engine.quit();
		return;
	} else {
		auto play = self._play->getComponent<TextComponent>();
		play->textRenderer->setText((entity == self._play) ? ">Play<" : " Play ");

		auto quit = self._quit->getComponent<TextComponent>();
		quit->textRenderer->setText((entity == self._quit) ? ">Quit<" : " Quit ");
	}
}
