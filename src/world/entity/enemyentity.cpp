#include "enemyentity.hpp"

#include "../component/transformcomponent.hpp"
#include "../component/modelcomponent.hpp"
#include "../component/kbmouseinputcomponent.hpp"
#include "../component/particlecomponent.hpp"
#include "../component/physicscomponent.hpp"

#include "../../engine.hpp"

EnemyEntity::EnemyEntity() : Entity(sole::rebuild("31bcc9bd-78bb-45b7-bb86-1917bcf5df6d"), "Enemy"){
	auto transform = addComponent<TransformComponent>();
	transform->scale = glm::vec3(0.3);
	transform->position = glm::vec3(0, 1, 5);
	transform->rotation = glm::vec3(0, 0, 0);
	transform->recalculateMatrix();
	auto model = addComponent<ModelComponent>();
	model->meshData = Engine::getInstance().getMeshLoader()->getMesh("assets/objects/enemy.fbx");
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
	auto particle = addComponent<ParticleComponent>();
	particle->addEmitter(glm::vec3(0, 1, 5), 1024);
	auto physics = addComponent<PhysicsComponent>();
}

void EnemyEntity::registerImGui() {
	
}