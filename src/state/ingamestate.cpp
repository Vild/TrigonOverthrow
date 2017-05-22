// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "ingamestate.hpp"

#include <glm/glm.hpp>
#include "../engine.hpp"

#include "../lib/glad.h"
#include "../lib/imgui.h"
#include "../gl/shader.hpp"

#include "../world/system/bulletphysicssystem.hpp"
#include "../world/system/floortilesystem.hpp"
#include "../world/system/roomloadingsystem.hpp"
#include "../world/system/ingamemenusystem.hpp"

#include "../world/component/transformcomponent.hpp"
#include "../world/component/cameracomponent.hpp"
#include "../world/component/lookatcomponent.hpp"
#include "../world/component/particlecomponent.hpp"
#include "../world/component/kbmouseinputcomponent.hpp"
#include "../world/component/textcomponent.hpp"
#include "../world/component/modelcomponent.hpp"
#include "../world/component/guncomponent.hpp"
#include "../world/component/lifecomponent.hpp"
#include "../world/component/instancedsimplemeshcomponent.hpp"
#include "../world/component/dynamicmodelcomponent.hpp"
#include "../world/component/suncomponent.hpp"
#include "../world/component/pointlightcomponent.hpp"
#include "../world/component/floortilecomponent.hpp"
#include "../world/component/hovercomponent.hpp"
#include "../world/component/upgradecomponent.hpp"
#include "../world/component/musiccomponent.hpp"
#include "../world/component/experiencecomponent.hpp"
#include "../world/component/experienceorbcomponent.hpp"
#include "../world/component/ingamemenucomponent.hpp"

InGameState::InGameState() {
	auto& engine = Engine::getInstance();
	BulletPhysicsSystem* bulletphyiscs = engine.getSystem<BulletPhysicsSystem>();

	_sun = _world.addEntity(sole::uuid4(), "Sun");
	_camera = _world.addEntity(sole::rebuild("f8bb5ea8-e3fb-4ec7-939d-5d70ae3e9d12"), "Camera");
	_player = _world.addEntity(sole::rebuild("31bcc9bd-78bb-45b7-bb86-1917bcf5df6d"), "Player");
	//_floor = _world.addEntity(sole::rebuild("b056cfea-b2cd-4c91-b921-5b8ee6b286d6"), "Floor");
	//_enemy = _world.addEntity(sole::uuid4(), "Enemy");

	//{
	//	std::shared_ptr<MapInformation> mapInfo = engine.getJSONLoader()->loadMap("assets/maps/smileyface.json");
	//	std::vector<Entity*> entities = mapInfo->constructEntities(_world);
	//	for (Entity* entity : entities) {
	//		if (!entity)
	//			continue;

	//		// XXX: hack
	//		auto transform = entity->getComponent<TransformComponent>();
	//		if (!transform)
	//			continue;
	//		auto rigidbody = entity->getComponent<RigidBodyComponent>();
	//		if (!rigidbody)
	//			continue;
	//		rigidbody->setHitboxHalfSize(transform->getScale());
	//		rigidbody->setTransform(transform);

	//		bulletphyiscs->addRigidBody(rigidbody, BulletPhysicsSystem::CollisionType::COL_ENEMY, BulletPhysicsSystem::enemyCollidesWith);
	//	}
	//}

	{ // Adding Sun
		auto sun = _sun->addComponent<SunComponent>();
		sun->ambient = glm::vec3(0);
		sun->directionLight.diffuse = glm::vec3(0.4, 0.4, 0.4);
		sun->directionLight.specular = glm::vec3(0.0, 0.0, 0.0);
		sun->directionLight.direction = glm::vec3(0, -1, 0);

		auto music = _sun->addComponent<MusicComponent>();
		music->music = engine.getAudioManager()->getMusic("assets/music/Mega Hyper Ultrastorm.ogg");
		music->music->volume = 4;
		music->music->play(-1);
	}

	{ // Adding Camera
		_camera->addComponent<TransformComponent>();
		_camera->addComponent<CameraComponent>();
		auto lookAt = _camera->addComponent<LookAtComponent>();
		lookAt->target = _player;
		lookAt->followMode = FollowMode::followByOffset;
		lookAt->offsetFromTarget = glm::vec3(0, 9, -5);

		_camera->registerImGui = &InGameState::_registerImGUI;
	}

	{ // Adding Player
		auto transform = _player->addComponent<TransformComponent>();
		transform->setPosition(glm::vec3(3));
		transform->setScale(glm::vec3(0.3));
		transform->setDirection({0, 0, 1});

		auto model = _player->addComponent<ModelComponent>();
		model->meshData = engine.getMeshLoader()->getMesh("assets/objects/player.fbx");
		model->meshData->texture = Engine::getInstance().getTextureManager()->getTexture("assets/textures/errorNormal.png");
		model->meshData->mesh
			->addBuffer("m",
									[](GLuint id) {
										glBindBuffer(GL_ARRAY_BUFFER, id);
										glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);

										for (int i = 0; i < 4; i++) {
											glEnableVertexAttribArray(ShaderAttributeID::m + i);
											glVertexAttribPointer(ShaderAttributeID::m + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4) * i));
											glVertexAttribDivisor(ShaderAttributeID::m + i, 1);
										}

										glBindBuffer(GL_ARRAY_BUFFER, 0);
									})
			.finalize();

		_player->addComponent<KBMouseInputComponent>();

		/*auto life = */ _player->addComponent<LifeComponent>();

		auto gun = _player->addComponent<GunComponent>();
		gun->addGun(GunComponent::GunType::RAYGUN, 0.35f); // 60 is the the cooldown rate per frame.

		auto expComp = _player->addComponent<ExperienceComponent>();
		expComp->currExp = 0;
		expComp->expToNextLevel = 5;
		expComp->pickUpRadius = 4.8;

		auto upgrades = _player->addComponent<UpgradeComponent>();
		upgrades->multipleRayMultiplier = 0;
		upgrades->reflectionCount = 0;
		upgrades->refractionCount = 1;

		auto text = _player->addComponent<TextComponent>();
		text->textRenderer = engine.getTextFactory()->makeRenderer("Hello, My name is Mr. Duck!\x01");
		text->transform.setPosition(glm::vec3(0, 1, 0));
		text->transform.setScale(glm::vec3(10));

		auto rigidbody = _player->addComponent<RigidBodyComponent>(_player, 1.0f, 1.0f);
		rigidbody->getRigidBody()->setDamping(0.9, 0);
		rigidbody->setHitboxHalfSize(transform->getScale());
		rigidbody->setTransform(transform);
		rigidbody->setActivationState(DISABLE_DEACTIVATION);
		bulletphyiscs->addRigidBody(rigidbody, BulletPhysicsSystem::CollisionType::COL_PLAYER, BulletPhysicsSystem::playerCollidesWith);

		auto point = _player->addComponent<PointLightComponent>();
		point->pointLight.diffuse = glm::vec3(0, 0.9, 0);
		point->pointLight.specular = glm::vec3(0, 0.05, 0);
		point->pointLight.constant = 1;
		point->pointLight.linear = 0.14;
		point->pointLight.quadratic = 0.07;

		_player->addComponent<HoverComponent>(0.6, 100);
	}
}

InGameState::~InGameState() {}

void InGameState::onEnter(State* prev) {
	auto music = _sun->getComponent<MusicComponent>();
	music->music->play(-1);

	InGameMenuSystem* menuSystem = Engine::getInstance().getSystem<InGameMenuSystem>();
	menuSystem->updateUI = &_updateUI;
}

void InGameState::onLeave(State* next) {
	auto music = _sun->getComponent<MusicComponent>();
	music->music->stop();
	InGameMenuSystem* menuSystem = Engine::getInstance().getSystem<InGameMenuSystem>();
	menuSystem->updateUI = nullptr;
}

void InGameState::registerImGui() {}

void InGameState::_addLookAt() {
	auto lookAt = _camera->addComponent<LookAtComponent>();
	lookAt->target = _player;
	lookAt->followMode = FollowMode::followByOffset;
	lookAt->offsetFromTarget = glm::vec3(0, 2.5, -5);
}

void InGameState::_registerImGUI(Entity& self, State& state) {
	static bool follow = true;
	InGameState& this_ = static_cast<InGameState&>(state);
	if (ImGui::Checkbox("Follow target", &follow)) {
		if (follow)
			this_._addLookAt();
		else
			self.removeComponent<LookAtComponent>();
	}
}

void InGameState::_updateUI(State& state, bool show) {
	Engine& engine = Engine::getInstance();
	InGameState& this_ = static_cast<InGameState&>(state);
	engine.getPause() = show;
	if (show) {
		auto menu = this_._sun->addComponent<InGameMenuComponent>();

		menu->renderUI = &_menuRenderUI;
	} else
		this_._sun->removeComponent<InGameMenuComponent>();
}

void InGameState::_menuRenderUI(Entity& self, State& state) {
	InGameState& this_ = static_cast<InGameState&>(state);
	Engine& engine = Engine::getInstance();
	Entity* player = this_.getPlayer();
	UpgradeComponent* upgrade = player->getComponent<UpgradeComponent>();
	ExperienceComponent* experience = player->getComponent<ExperienceComponent>();

	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
																 ImGuiWindowFlags_NoCollapse /* | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar */;
	ImGui::PushFont(engine.getMediumFont());
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1, 1, 1, 1));
	ImGui::PushStyleColor(ImGuiCol_ModalWindowDarkening, ImVec4(0.1, 0.15, 0.15, 0.75));

	ImGui::SetNextWindowPosCenter(ImGuiSetCond_Always);
	// ImGui::SetNextWindowSize(ImVec2(800, 200), ImGuiSetCond_Always);
	// float scale = 1.0 - (512.0 / engine.getWidth());

	ImGui::OpenPopup("Trigon paused");
	if (ImGui::BeginPopupModal("Trigon paused", nullptr, windowFlags)) {
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 16);

		ImGui::Text("Upgrade points: %d", upgrade->upgradePoints);
		ImGui::Text("%d/%d xp until next upgrade point", experience->currExp, experience->expToNextLevel);
		ImGui::Separator();

		bool enableShop = !!upgrade->upgradePoints;

		ImVec4 enableButton = ImVec4(0.1, 0.5, 0.2, 1);
		ImVec4 enableButtonHovered = ImVec4(0.3, 0.8, 0.3, 1);
		ImVec4 enableButtonActive = ImVec4(0.95, 0.95, 0.95, 1);
		ImVec4 disableButton = ImVec4(0.6, 0.6, 0.6, 1);
		ImVec4 disableButtonHovered = ImVec4(0.6, 0.6, 0.6, 1);
		ImVec4 disableButtonActive = ImVec4(0.6, 0.6, 0.6, 1);

#define BUTTON_COLORS(enable) \
	(enable) ? enableButton : disableButton, (enable) ? enableButtonHovered : disableButtonHovered, (enable) ? enableButtonActive : disableButtonActive

		ImGui::Columns(2);
		ImGui::SetColumnOffset(1, 385);

		ImGui::Text("Multiple Ray Multiplier: %d", upgrade->multipleRayMultiplier);
		ImGui::NextColumn();
		bool enabled = enableShop && upgrade->multipleRayMultiplier < UpgradeComponent::MAX_EXTRA_RAYS;
		if (ImGui::ButtonWithColor("+##MRM", ImVec2(0, 0), BUTTON_COLORS(enabled)) && enabled) {
			upgrade->multipleRayMultiplier++;
			upgrade->upgradePoints--;
		}
		ImGui::NextColumn();

		ImGui::Text("Reflection Count: %d", upgrade->reflectionCount);
		ImGui::NextColumn();
		enabled = enableShop && upgrade->reflectionCount < UpgradeComponent::MAX_REFLECT_LIMIT;
		if (ImGui::ButtonWithColor("+##RC", ImVec2(0, 0), BUTTON_COLORS(enabled)) && enabled) {
			upgrade->reflectionCount++;
			upgrade->upgradePoints--;
		}
		ImGui::NextColumn();

		ImGui::Text("Refraction Count: %d", upgrade->refractionCount);
		ImGui::NextColumn();
		enabled = enableShop && upgrade->refractionCount < UpgradeComponent::MAX_REFRACTION_LIMIT;
		if (ImGui::ButtonWithColor("+##RC2", ImVec2(0, 0), BUTTON_COLORS(enabled)) && enabled) {
			upgrade->refractionCount++;
			upgrade->upgradePoints--;
		}
		ImGui::NextColumn();

		ImGui::Columns(1);

		ImGui::Separator();
		if (ImGui::Button("Continue", ImVec2(200, 0)))
			engine.getSystem<InGameMenuSystem>()->escapePressed = true; // :3
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_ModalWindowDarkening, ImVec4(0.1, 0, 0, 0.75));
		if (ImGui::ButtonWithColor("Quit", ImVec2(200, 0), ImVec4(0.5, 0, 0, 1), ImVec4(0.8, 0, 0, 1), ImVec4(0.95, 0, 0, 1)))
			ImGui::OpenPopup("Quit?");
		if (ImGui::BeginPopupModal("Quit?", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text("All that beautiful progress will be lost.\nThis operation cannot be undone!\n\n");
			ImGui::Separator();

			if (ImGui::ButtonWithColor("Yes", ImVec2(250, 0), ImVec4(0.5, 0, 0, 1), ImVec4(0.8, 0, 0, 1), ImVec4(0.95, 0, 0, 1)))
				engine.quit();
			ImGui::SameLine();

			if (ImGui::ButtonWithColor("No", ImVec2(250, 0), ImVec4(0, 0.5, 0, 1), ImVec4(0, 0.8, 0, 1), ImVec4(0, 0.95, 0, 1)))
				ImGui::CloseCurrentPopup();

			ImGui::EndPopup();
		}
		ImGui::PopStyleColor(1);

		ImGui::PopStyleVar();
		ImGui::EndPopup();
	}

	ImGui::PopStyleColor(2);
	ImGui::PopFont();
}