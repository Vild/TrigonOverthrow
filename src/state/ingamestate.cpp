#include "ingamestate.hpp"

#include <glm/glm.hpp>
#include "../engine.hpp"

#include "../lib/glad.h"
#include "../gl/shader.hpp"

#include "../world/component/transformcomponent.hpp"
#include "../world/component/cameracomponent.hpp"
#include "../world/component/lookatcomponent.hpp"
#include "../world/component/particlecomponent.hpp"
#include "../world/component/kbmouseinputcomponent.hpp"
#include "../world/component/physicscomponent.hpp"
#include "../world/component/textcomponent.hpp"
#include "../world/component/floortransformcomponent.hpp"
#include "../world/component/modelcomponent.hpp"

InGameState::InGameState() {
	auto& engine = Engine::getInstance();

	std::shared_ptr<Entity> camera;
	std::shared_ptr<Entity> player;
	std::shared_ptr<Entity> floor;

	{ // Adding Camera
		camera = std::make_shared<Entity>(sole::rebuild("f8bb5ea8-e3fb-4ec7-939d-5d70ae3e9d12"), "Camera");

		auto transform = camera->addComponent<TransformComponent>();
		auto cameraComponent = camera->addComponent<CameraComponent>();
		auto lookAt = camera->addComponent<LookAtComponent>();
		lookAt->followMode = FollowMode::followByOffset;
		lookAt->offsetFromTarget = glm::vec3(0, 2.5, -5);
	}

	{ // Adding Player
		player = std::make_shared<Entity>(sole::rebuild("31bcc9bd-78bb-45b7-bb86-1917bcf5df6d"), "Player");

		auto transform = player->addComponent<TransformComponent>();
		transform->scale = glm::vec3(0.01);
		transform->recalculateMatrix();
		auto model = player->addComponent<ModelComponent>();
		model->meshData = engine.getMeshLoader()->getMesh("assets/objects/player.fbx");
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
		auto particle = player->addComponent<ParticleComponent>();
		particle->addEmitter(glm::vec3(0, 1, 0), 1024);
		auto input = player->addComponent<KBMouseInputComponent>();
		auto physics = player->addComponent<PhysicsComponent>();
		auto text = player->addComponent<TextComponent>();
		text->textRenderer = engine.getTextFactory()->makeRenderer("Hello, My name is Mr. Duck!\x01");

		text->transform.position = glm::vec3(0, 2, 0);
		text->transform.scale = glm::vec3(100 * 2); // To counteract transform->scale
		text->transform.recalculateMatrix();
	}

	{ // Adding Floor
		floor = std::make_shared<Entity>(sole::rebuild("b056cfea-b2cd-4c91-b921-5b8ee6b286d6"), "Floor");

		constexpr int gridSize = 8; // will be gridSize*gridSize

		auto transform = floor->addComponent<FloorTransformComponent>();
		transform->gridSize = gridSize;
		transform->scale = glm::vec3(1, 0.1, 1);
		transform->recalculateMatrices();

#define frand() ((rand() * 1.0) / RAND_MAX)
		float* topData = new float[gridSize * gridSize];

		for (int z = 0; z < gridSize; z++)
			for (int x = 0; x < gridSize; x++) {
				topData[z * gridSize + x] = frand() * 2 - 1;
			}

		for (int z = 0; z < gridSize; z++)
			for (int x = 0; x < gridSize; x++) {
				auto p = topData[z * gridSize + x];
				const auto& forwards = z > 0 ? topData[(z - 1) * gridSize + x] : p;
				const auto& left = x < gridSize - 1 ? topData[z * gridSize + x + 1] : p;
				const auto& right = x > 0 ? topData[z * gridSize + x - 1] : p;
				const auto& backwards = z < gridSize - 1 ? topData[(z + 1) * gridSize + x] : p;

				p = ((forwards + left + right + backwards) + p * 2) / 6;
			}

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

		auto model = floor->addComponent<ModelComponent>();
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

	camera->getComponent<LookAtComponent>()->target = player.get();
	_world.addEntity(camera);
	_world.addEntity(player);
	_world.addEntity(floor);

	engine.getCamera() = camera;
}
