#include "playerentity.hpp"

#include "../component/transformcomponent.hpp"
#include "../component/modelcomponent.hpp"
#include "../component/kbmouseinputcomponent.hpp"

#include "../../engine.hpp"

PlayerEntity::PlayerEntity() : Entity(sole::rebuild("31bcc9bd-78bb-45b7-bb86-1917bcf5df6d"), "Player") {
	auto transform = addComponent<TransformComponent>();
	transform->scale = glm::vec3(0.001);
	auto model = addComponent<ModelComponent>();
	model->meshData = Engine::getInstance().getMeshLoader()->getMesh("assets/objects/player.fbx");

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
	auto input = addComponent<KBMouseInputComponent>();
}
