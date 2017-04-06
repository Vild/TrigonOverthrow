#include "playerentity.hpp"

#include "../component/transformcomponent.hpp"
#include "../component/modelcomponent.hpp"
#include "../component/kbmouseinputcomponent.hpp"
#include "../component/particlecomponent.hpp"


#include "../../engine.hpp"

PlayerEntity::PlayerEntity() : Entity(sole::rebuild("31bcc9bd-78bb-45b7-bb86-1917bcf5df6d"), "Player") {
	auto transform = addComponent<TransformComponent>();
	transform->scale = glm::vec3(0.001);
	auto model = addComponent<ModelComponent>();
	model->meshData = Engine::getInstance().getMeshLoader()->getMesh("assets/objects/player.fbx");
	auto input = addComponent<KBMouseInputComponent>();

	auto particle = addComponent<ParticleComponent>();
	particle->addEmitter(glm::vec3(0, 1, 0));
}
