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

InGameState::InGameState() {
	auto& engine = Engine::getInstance();
	BulletPhysicsSystem * bulletphyiscs = engine.getSystem<BulletPhysicsSystem>();
	

	_camera = _world.addEntity(sole::rebuild("f8bb5ea8-e3fb-4ec7-939d-5d70ae3e9d12"), "Camera");
	_player = _world.addEntity(sole::rebuild("31bcc9bd-78bb-45b7-bb86-1917bcf5df6d"), "Player");
	_floor = _world.addEntity(sole::rebuild("b056cfea-b2cd-4c91-b921-5b8ee6b286d6"), "Floor");
	_enemy = _world.addEntity(sole::uuid4(), "Enemy");

	{ // Adding Camera
		_camera->addComponent<TransformComponent>();
		_camera->addComponent<CameraComponent>();
		_addLookAt();
		_camera->registerImGui = &InGameState::_registerImGUI;
	}

	{ // Adding Player
		auto transform = _player->addComponent<TransformComponent>();
		transform->setScale(glm::vec3(0.3));
		transform->setDirection({ 0,1,0 });

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

		auto life = _player->addComponent<LifeComponent>();

		auto gun = _player->addComponent<GunComponent>();
		gun->addGun(GunComponent::GunType::RAYGUN, 30); // 60 is the the cooldown rate per frame.

		auto text = _player->addComponent<TextComponent>();
		text->textRenderer = engine.getTextFactory()->makeRenderer("Hello, My name is Mr. Duck!\x01");
		text->transform.setPosition(glm::vec3(0, 200, 0));
		text->transform.setScale(glm::vec3(100 * 2)); // To counteract transform->scale

		auto rigidbody = _player->addComponent<RigidBodyComponent>();
		rigidbody->setHitboxHalfSize(transform->getScale());
		rigidbody->setMass(1);
		//rigidbody->setFriction(2);
		rigidbody->setTransform(transform);

		bulletphyiscs->addRigidBody(rigidbody);
	}

	{
		auto transform = _enemy->addComponent<TransformComponent>();
		transform->setScale(glm::vec3(0.3));
		transform->setPosition(glm::vec3(0, 0.2, 5));

		auto model = _enemy->addComponent<ModelComponent>();
		model->meshData = engine.getMeshLoader()->getMesh("assets/objects/enemy.fbx");
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

		auto hitbox = _enemy->addComponent<HitboxComponent>();
		hitbox->addHitbox(HitboxComponent::SPHERE, transform->getPosition());
		_enemy->addComponent<PhysicsComponent>();
		
		auto life = _enemy->addComponent<LifeComponent>();
		life->currHP = life->maxHP = 5;

		auto text = _enemy->addComponent<TextComponent>();
		text->textRenderer = engine.getTextFactory()->makeRenderer("Hello, I am a Trigoon, prepare to die!\x01");

		text->transform.setPosition({ 0, 2, 5 });
		//text->transform.rotation = glm::vec3(0, 0, 0);
		text->transform.setScale({0.1, 0.1, 0.1}); // To counteract transform->scale

		auto rigidbody = _enemy->addComponent<RigidBodyComponent>();

		rigidbody->setHitboxHalfSize(transform->getScale());
		rigidbody->setMass(3);
		rigidbody->setFriction(1);

		bulletphyiscs->addRigidBody(rigidbody);
	}

	{ // Adding Floor
		// How to fix support for non-uniform sizes of the map. E.g 2 in height and 6 in width.
		std::vector<Uint8> map = Engine::getInstance().getMapLoader()->getMap("maps/smileyface.png");
		auto transform = _floor->addComponent<FloorTransformComponent>();
		transform->gridSize = Engine::getInstance().getMapLoader()->getHeight();
		transform->scale = glm::vec3(1, 0.1, 1);
		transform->recalculateMatrices();

		int gridSize = transform->gridSize;

#define frand() ((rand() * 1.0) / RAND_MAX)
		float* topData = new float[gridSize * gridSize];

		for (int z = 0; z < gridSize; z++)
			for (int x = 0; x < gridSize; x++) {
				topData[z * gridSize + x] = float(map[z * gridSize + x] / float(5));
			}

		// for (int z = 0; z < gridSize; z++)
		//	for (int x = 0; x < gridSize; x++) {
		//		auto p = topData[z * gridSize + x];
		//		const auto& forwards = z > 0 ? topData[(z - 1) * gridSize + x] : p;
		//		const auto& left = x < gridSize - 1 ? topData[z * gridSize + x + 1] : p;
		//		const auto& right = x > 0 ? topData[z * gridSize + x - 1] : p;
		//		const auto& backwards = z < gridSize - 1 ? topData[(z + 1) * gridSize + x] : p;
		//
		//		p = ((forwards + left + right + backwards) + p * 2) / 6;
		//	}

		glm::vec4* neighborData = new glm::vec4[gridSize * gridSize];
		for (int z = 0; z < gridSize; z++)
			for (int x = 0; x < gridSize; x++) {
				// See floorbase.geom
				enum Side { forwards = 0, left = 1, right = 2, backwards = 3 };

				glm::vec4& point = neighborData[z * gridSize + x];

				float cur = topData[z * gridSize + x];

				float minFloor = cur - 100;

				point[forwards] = z > 0 ? topData[(z - 1) * gridSize + x] : minFloor;
				point[left] = x < gridSize - 1 ? topData[z * gridSize + x + 1] : minFloor;
				point[right] = x > 0 ? topData[z * gridSize + x - 1] : minFloor;
				point[backwards] = z < gridSize - 1 ? topData[(z + 1) * gridSize + x] : minFloor;

				point[forwards] = std::min(minFloor, point[forwards]);
				point[left] = std::min(minFloor, point[left]);
				point[right] = std::min(minFloor, point[right]);
				point[backwards] = std::min(minFloor, point[backwards]);
			}

		auto model = _floor->addComponent<ModelComponent>();
		model->meshData = engine.getMeshLoader()->getMesh("assets/objects/box.obj");
		model->meshData->mesh
			->addBuffer("m",
									[&](GLuint id) {
										glBindBuffer(GL_ARRAY_BUFFER, id);
										glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * gridSize * gridSize, NULL, GL_DYNAMIC_DRAW);

										for (int i = 0; i < 4; i++) {
											glEnableVertexAttribArray(ShaderAttributeID::m + i);
											glVertexAttribPointer(ShaderAttributeID::m + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4) * i));
											glVertexAttribDivisor(ShaderAttributeID::m + i, 1);
										}

										glBindBuffer(GL_ARRAY_BUFFER, 0);
									})
			.addBuffer("top",
								 [&](GLuint id) {
									 glBindBuffer(GL_ARRAY_BUFFER, id);
									 glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * gridSize * gridSize, topData, GL_STATIC_DRAW);

									 glEnableVertexAttribArray(ShaderAttributeID::top);
									 glVertexAttribPointer(ShaderAttributeID::top, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), NULL);
									 glVertexAttribDivisor(ShaderAttributeID::top, 1);

									 glBindBuffer(GL_ARRAY_BUFFER, 0);
								 })
			.addBuffer("neighbor",
								 [&](GLuint id) {
									 glBindBuffer(GL_ARRAY_BUFFER, id);
									 glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * gridSize * gridSize, neighborData, GL_STATIC_DRAW);

									 glEnableVertexAttribArray(ShaderAttributeID::neighbor);
									 glVertexAttribPointer(ShaderAttributeID::neighbor, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), NULL);
									 glVertexAttribDivisor(ShaderAttributeID::neighbor, 1);

									 glBindBuffer(GL_ARRAY_BUFFER, 0);
								 })
			.finalize();
		delete[] topData;
		delete[] neighborData;
	}
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