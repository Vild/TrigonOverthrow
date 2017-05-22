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
	meshes[7] = Engine::getInstance().getMeshLoader()->getMesh("assets/objects/enemy_8HP.fbx");
	meshes[8] = Engine::getInstance().getMeshLoader()->getMesh("assets/objects/enemy_9HP.fbx");
	meshes[9] = Engine::getInstance().getMeshLoader()->getMesh("assets/objects/enemy_10HP.fbx");
	meshes[10] = Engine::getInstance().getMeshLoader()->getMesh("assets/objects/enemy_11HP.fbx");
	meshes[11] = Engine::getInstance().getMeshLoader()->getMesh("assets/objects/enemy_12HP.fbx");
	meshes[12] = Engine::getInstance().getMeshLoader()->getMesh("assets/objects/enemy_13HP.fbx");
	meshes[13] = Engine::getInstance().getMeshLoader()->getMesh("assets/objects/enemy_14HP.fbx");
	meshes[14] = Engine::getInstance().getMeshLoader()->getMesh("assets/objects/enemy_15HP.fbx");
	meshes[15] = Engine::getInstance().getMeshLoader()->getMesh("assets/objects/enemy_16HP.fbx");
	meshes[16] = Engine::getInstance().getMeshLoader()->getMesh("assets/objects/enemy_17HP.fbx");
	meshes[17] = Engine::getInstance().getMeshLoader()->getMesh("assets/objects/enemy_18HP.fbx");
	meshes[18] = Engine::getInstance().getMeshLoader()->getMesh("assets/objects/enemy_19HP.fbx");
	meshes[19] = Engine::getInstance().getMeshLoader()->getMesh("assets/objects/enemy_20HP.fbx");

	for (int i = 0; i < 20; i++) {
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

DynamicModelComponent::DynamicModelComponent(const ComponentValues& value) /* TODO: Re-add this when class is fixed: DynamicModelComponent() */ {
	meshes[0] = Engine::getInstance().getMeshLoader()->getMesh(value.getString("mesh0", "assets/objects/enemy_1HP.fbx"));
	meshes[1] = Engine::getInstance().getMeshLoader()->getMesh(value.getString("mesh1", "assets/objects/enemy_2HP.fbx"));
	meshes[2] = Engine::getInstance().getMeshLoader()->getMesh(value.getString("mesh2", "assets/objects/enemy_3HP.fbx"));
	meshes[3] = Engine::getInstance().getMeshLoader()->getMesh(value.getString("mesh3", "assets/objects/enemy_4HP.fbx"));
	meshes[4] = Engine::getInstance().getMeshLoader()->getMesh(value.getString("mesh4", "assets/objects/enemy_5HP.fbx"));
	meshes[5] = Engine::getInstance().getMeshLoader()->getMesh(value.getString("mesh5", "assets/objects/enemy_6HP.fbx"));
	meshes[6] = Engine::getInstance().getMeshLoader()->getMesh(value.getString("mesh6", "assets/objects/enemy_7HP.fbx"));
	meshes[7] = Engine::getInstance().getMeshLoader()->getMesh(value.getString("mesh7", "assets/objects/enemy_8HP.fbx"));
	meshes[8] = Engine::getInstance().getMeshLoader()->getMesh(value.getString("mesh8", "assets/objects/enemy_9HP.fbx"));
	meshes[9] = Engine::getInstance().getMeshLoader()->getMesh(value.getString("mesh9", "assets/objects/enemy_10HP.fbx"));
	meshes[10] = Engine::getInstance().getMeshLoader()->getMesh(value.getString("mesh10", "assets/objects/enemy_11HP.fbx"));
	meshes[11] = Engine::getInstance().getMeshLoader()->getMesh(value.getString("mesh11", "assets/objects/enemy_12HP.fbx"));
	meshes[12] = Engine::getInstance().getMeshLoader()->getMesh(value.getString("mesh12", "assets/objects/enemy_13HP.fbx"));
	meshes[13] = Engine::getInstance().getMeshLoader()->getMesh(value.getString("mesh13", "assets/objects/enemy_14HP.fbx"));
	meshes[14] = Engine::getInstance().getMeshLoader()->getMesh(value.getString("mesh14", "assets/objects/enemy_15HP.fbx"));
	meshes[15] = Engine::getInstance().getMeshLoader()->getMesh(value.getString("mesh15", "assets/objects/enemy_16HP.fbx"));
	meshes[16] = Engine::getInstance().getMeshLoader()->getMesh(value.getString("mesh16", "assets/objects/enemy_17HP.fbx"));
	meshes[17] = Engine::getInstance().getMeshLoader()->getMesh(value.getString("mesh17", "assets/objects/enemy_18HP.fbx"));
	meshes[18] = Engine::getInstance().getMeshLoader()->getMesh(value.getString("mesh18", "assets/objects/enemy_19HP.fbx"));
	meshes[19] = Engine::getInstance().getMeshLoader()->getMesh(value.getString("mesh19", "assets/objects/enemy_20HP.fbx"));

	for (int i = 0; i < 20; i++) {
		meshes[i]->texture = Engine::getInstance().getTextureManager()->getTexture(value.getString("texture", "assets/textures/errorNormal.png"));
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

void DynamicModelComponent::registerImGui() {}
