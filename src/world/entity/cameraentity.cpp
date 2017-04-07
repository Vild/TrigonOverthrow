#include "cameraentity.hpp"

#include "../component/transformcomponent.hpp"
#include "../component/lookatcomponent.hpp"
#include "../component/cameracomponent.hpp"

CameraEntity::CameraEntity() : Entity(sole::rebuild("f8bb5ea8-e3fb-4ec7-939d-5d70ae3e9d12"), "Camera") {
	auto transform = addComponent<TransformComponent>();
	transform->position = { 0, 0, -10 };
	transform->recalculateMatrix();

	auto cameraComponent = addComponent<CameraComponent>();

	/*auto lookAt = addComponent<LookAtComponent>();
	lookAt->followMode = FollowMode::followByOffset;
	lookAt->offsetFromTarget = glm::vec3(0, 1, -2);*/
}
