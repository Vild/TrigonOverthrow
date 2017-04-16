// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "playerentity.hpp"

#include "../component/transformcomponent.hpp"
#include "../component/modelcomponent.hpp"
#include "../component/kbmouseinputcomponent.hpp"
#include "../component/particlecomponent.hpp"
#include "../component/physicscomponent.hpp"
#include "../component/textcomponent.hpp"

#include "../../engine.hpp"

PlayerEntity::PlayerEntity() : Entity(sole::rebuild("31bcc9bd-78bb-45b7-bb86-1917bcf5df6d"), "Player") {
	auto& engine = Engine::getInstance();
	auto transform = addComponent<TransformComponent>();
	transform->scale = glm::vec3(0.01);
	transform->recalculateMatrix();
	auto model = addComponent<ModelComponent>();
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
	auto particle = addComponent<ParticleComponent>();
	particle->addEmitter(glm::vec3(0, 1, 0), 1024);
	auto input = addComponent<KBMouseInputComponent>();
	auto physics = addComponent<PhysicsComponent>();
	auto text = addComponent<TextComponent>();
	text->textRenderer = engine.getTextFactory()->makeRenderer("Sample text here");
}

void PlayerEntity::registerImGui() {}
