// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "dynamicmodelcomponent.hpp"
#include "../../engine.hpp"

DynamicModelComponent::DynamicModelComponent() {
	meshes[0] = Engine::getInstance().getMeshLoader()->getMesh("assets/objects/enemy_1HP.fbx");
	meshes[1] = Engine::getInstance().getMeshLoader()->getMesh("assets/objects/enemy_2HP.fbx");
	meshes[2] = Engine::getInstance().getMeshLoader()->getMesh("assets/objects/enemy_3HP.fbx");
	meshes[3] = Engine::getInstance().getMeshLoader()->getMesh("assets/objects/enemy_4HP.fbx");
	meshes[4] = Engine::getInstance().getMeshLoader()->getMesh("assets/objects/enemy_5HP.fbx");
	meshes[5] = Engine::getInstance().getMeshLoader()->getMesh("assets/objects/enemy_6HP.fbx");
	meshes[6] = Engine::getInstance().getMeshLoader()->getMesh("assets/objects/enemy_7HP.fbx");

	for (int i = 0; i < 7; i++) {
		meshes[i]->texture = Engine::getInstance().getTextureManager()->getTexture("assets/textures/errorNormal.png");
		meshes[i]
			->mesh
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
	}
}

DynamicModelComponent::DynamicModelComponent(const ComponentValues& value) : DynamicModelComponent() {
	// TODO:
}

void DynamicModelComponent::registerImGui() {}
