#include "ingamestate.hpp"

#include <glm/glm.hpp>
#include "../engine.hpp"

#include "../lib/glad.h"
#include "../lib/imgui.h"
#include "../gl/shader.hpp"

#include "../world/system/bulletphysicssystem.hpp"

#include "../world/component/transformcomponent.hpp"
#include "../world/component/cameracomponent.hpp"
#include "../world/component/lookatcomponent.hpp"
#include "../world/component/particlecomponent.hpp"
#include "../world/component/kbmouseinputcomponent.hpp"
#include "../world/component/physicscomponent.hpp"
#include "../world/component/textcomponent.hpp"
#include "../world/component/floortransformcomponent.hpp"
#include "../world/component/modelcomponent.hpp"
#include "../world/component/hitboxcomponent.hpp"
#include "../world/component/guncomponent.hpp"
#include "../world/component/lifecomponent.hpp"
#include "../world/component/instancedsimplemeshcomponent.hpp"
#include "../world/component/dynamicmodelcomponent.hpp"
#include "../world/component/suncomponent.hpp"
#include "../world/component/pointlightcomponent.hpp"

InGameState::InGameState() {
	auto& engine = Engine::getInstance();
	BulletPhysicsSystem * bulletphyiscs = engine.getSystem<BulletPhysicsSystem>();

	_sun = _world.addEntity(sole::uuid4(), "Sun");
	_camera = _world.addEntity(sole::rebuild("f8bb5ea8-e3fb-4ec7-939d-5d70ae3e9d12"), "Camera");
	_player = _world.addEntity(sole::rebuild("31bcc9bd-78bb-45b7-bb86-1917bcf5df6d"), "Player");
	_floor = _world.addEntity(sole::rebuild("b056cfea-b2cd-4c91-b921-5b8ee6b286d6"), "Floor");
	_enemy = _world.addEntity(sole::uuid4(), "Enemy");

	{ // Adding Sun
		auto sun = _sun->addComponent<SunComponent>();
		sun->ambient = glm::vec3(0.1);
		sun->directionLight.diffuse = glm::vec3(0, 0.3, 0.3);
		sun->directionLight.specular = glm::vec3(0.3, 0, 0);
		sun->directionLight.direction = glm::vec3(0, -1, 0);
	}

	{ // Adding Camera
		_camera->addComponent<TransformComponent>();
		_camera->addComponent<CameraComponent>();
		_addLookAt();
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
		auto particle = _player->addComponent<ParticleComponent>();
		particle->addEmitter(transform->getDirection(), 1024);
		_player->addComponent<KBMouseInputComponent>();
		//_player->addComponent<PhysicsComponent>();

		/*auto life = */_player->addComponent<LifeComponent>();

		auto gun = _player->addComponent<GunComponent>();
		gun->addGun(GunComponent::GunType::RAYGUN, 30); // 60 is the the cooldown rate per frame.

		auto text = _player->addComponent<TextComponent>();
		text->textRenderer = engine.getTextFactory()->makeRenderer("Hello, My name is Mr. Duck!\x01");
		text->transform.setPosition(glm::vec3(0, 200, 0));
		text->transform.setScale(glm::vec3(100 * 2)); // To counteract transform->scale

		auto rigidbody = _player->addComponent<RigidBodyComponent>(_player, 1.0f, 1.0f);
		rigidbody->setHitboxHalfSize(transform->getScale());
		rigidbody->setTransform(transform);
		rigidbody->setActivationState(DISABLE_DEACTIVATION);
		bulletphyiscs->addRigidBody(rigidbody,
			BulletPhysicsSystem::CollisionType::COL_PLAYER,
			BulletPhysicsSystem::playerCollidesWith);

		auto point = _player->addComponent<PointLightComponent>();
		point->pointLight.diffuse = glm::vec3(0, 1, 0);
		point->pointLight.specular = glm::vec3(0, 0, 0);
		point->pointLight.constant = 1;
		point->pointLight.linear = 0.14;
		point->pointLight.quadratic = 0.07;
	}

	{
		auto transform = _enemy->addComponent<TransformComponent>();
		transform->setScale(glm::vec3(0.3));
		transform->setPosition(glm::vec3(0, 0.2, 5));

		/*auto dynamicModelComp = */_enemy->addComponent<DynamicModelComponent>();

		auto model = _enemy->addComponent<ModelComponent>();
		model->meshData = engine.getMeshLoader()->getMesh("assets/objects/enemy_7HP.fbx");
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

		//auto hitbox = _enemy->addComponent<HitboxComponent>();
		//hitbox->addHitbox(HitboxComponent::SPHERE, transform->getPosition());
		//_enemy->addComponent<PhysicsComponent>();

		auto life = _enemy->addComponent<LifeComponent>();
		life->currHP = life->maxHP = 6;

		auto text = _enemy->addComponent<TextComponent>();
		text->textRenderer = engine.getTextFactory()->makeRenderer("Hello, I am a Trigoon, prepare to die!\x01");

		text->transform.setPosition({0, 2, 5});
		// text->transform.rotation = glm::vec3(0, 0, 0);
		text->transform.setScale({0.1, 0.1, 0.1}); // To counteract transform->scale

		auto rigidbody = _enemy->addComponent<RigidBodyComponent>(_enemy, 1.0f, 1.0f);
		rigidbody->setHitboxHalfSize(transform->getScale());
		rigidbody->setTransform(transform);

		bulletphyiscs->addRigidBody(rigidbody,
			BulletPhysicsSystem::CollisionType::COL_ENEMY,
			BulletPhysicsSystem::enemyCollidesWith);
	}
	// clang-format off
	{ // Adding Floor
		auto mapLoader = engine.getMapLoader();
		std::vector<Uint8> map = mapLoader->getMap("maps/smileyface.png");
		int width = mapLoader->getWidth();
		int height = mapLoader->getHeight();

		Entity * room = _world.addEntity(sole::uuid4(), "Room");
		//
		std::unique_ptr<SimpleMesh> box = std::make_unique<SimpleMesh>();
		box->setDrawMode(GL_TRIANGLES)
		//	// TOP
			.addVertex({ -0.5,  0.5,  0.5 })
			.addVertex({  0.5,  0.5,  0.5 })
			.addVertex({ -0.5,  0.5, -0.5 })
			.addVertex({ -0.5,  0.5, -0.5 })
			.addVertex({  0.5,  0.5,  0.5 })
			.addVertex({  0.5,  0.5, -0.5 })
			// RIGHT
			.addVertex({ -0.5, -0.5,  0.5 })
			.addVertex({ -0.5,  0.5,  0.5 })
			.addVertex({ -0.5, -0.5, -0.5 })
			.addVertex({ -0.5, -0.5, -0.5 })
			.addVertex({ -0.5,  0.5,  0.5 })
			.addVertex({ -0.5,  0.5, -0.5 })
			// LEFT
			.addVertex({  0.5, -0.5,  0.5 })
			.addVertex({  0.5, -0.5, -0.5 })
			.addVertex({  0.5,  0.5,  0.5 })
			.addVertex({  0.5, -0.5, -0.5 })
			.addVertex({  0.5,  0.5, -0.5 })
			.addVertex({  0.5,  0.5,  0.5 })
			// FRONT
			.addVertex({ -0.5,  0.5, -0.5 })
			.addVertex({  0.5,  0.5, -0.5 })
			.addVertex({ -0.5, -0.5, -0.5 })
			.addVertex({ -0.5, -0.5, -0.5 })
			.addVertex({  0.5,  0.5, -0.5 })
			.addVertex({  0.5, -0.5, -0.5 })
			// BACK
			.addVertex({ -0.5,  0.5,  0.5 })
			.addVertex({  0.5,  0.5,  0.5 })
			.addVertex({ -0.5, -0.5,  0.5 })
			.addVertex({ -0.5, -0.5,  0.5 })
			.addVertex({  0.5,  0.5,  0.5 })
			.addVertex({  0.5, -0.5,  0.5 })
		.finalize(width * height);

		auto ismc = room->addComponent<InstancedSimpleMeshComponent>(std::move(box));

		for (unsigned int i = 0; i < map.size(); i++)
		{
			int x = i % width;
			int y = i / width;
			float h = float(map[i]) / 255.0f;

			Entity * tile  = _world.addEntity(sole::uuid4(), "FloorTile");
			tile->getHide() = true;

			TransformComponent * transform = tile->addComponent<TransformComponent>();
			transform->setPosition({ x, h - 0.5, y });
			transform->setScale({ 1, 1, 1 });
			ismc->addInstance(transform);

			RigidBodyComponent * rigidbody = tile->addComponent<RigidBodyComponent>(tile);
			rigidbody->setTransform(transform);
			rigidbody->setHitboxHalfSize({ 0.5, 0.5, 0.5 });

			bulletphyiscs->addRigidBody(rigidbody,
				BulletPhysicsSystem::CollisionType::COL_WALL,
				BulletPhysicsSystem::wallCollidesWith);
		}
	}
	// clang-format off
}

InGameState::~InGameState() {}

void InGameState::onEnter(State* prev) {}
void InGameState::onLeave(State* next) {}

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
