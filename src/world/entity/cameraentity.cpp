// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "cameraentity.hpp"

#include "../component/transformcomponent.hpp"
#include "../component/lookatcomponent.hpp"
#include "../component/cameracomponent.hpp"

CameraEntity::CameraEntity() : Entity(sole::rebuild("f8bb5ea8-e3fb-4ec7-939d-5d70ae3e9d12"), "Camera") {
	auto transform = addComponent<TransformComponent>();

	auto cameraComponent = addComponent<CameraComponent>();

	auto lookAt = addComponent<LookAtComponent>();
	lookAt->followMode = FollowMode::followByOffset;
	lookAt->offsetFromTarget = glm::vec3(0, 2.5, -5);
}
