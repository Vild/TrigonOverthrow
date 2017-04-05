#include "cameraentity.hpp"

#include "../component/transformcomponent.hpp"
#include "../component/lookatcomponent.hpp"

CameraEntity::CameraEntity() : Entity(sole::rebuild("f8bb5ea8-e3fb-4ec7-939d-5d70ae3e9d12"), "Camera") {
	auto transform = addComponent<TransformComponent>();
	auto lookAt = addComponent<LookAtComponent>();
	lookAt->followMode = FollowMode::followByOffset;
	lookAt->offsetFromTarget = glm::vec3(0, 1, -2);
}
