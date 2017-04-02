#include "player.hpp"

#include "../component/transform.hpp"
#include "../component/model.hpp"

#include "../../engine.hpp"

Player::Player() : Entity(sole::rebuild("31bcc9bd-78bb-45b7-bb86-1917bcf5df6d"), "Player") {
	auto transform = addComponent<TransformComponent>();
	transform->scale = glm::vec3(0.001);
	auto model = addComponent<ModelComponent>();
	model->meshData = Engine::getInstance().getMeshLoader()->getMesh("assets/objects/player.fbx");
}
