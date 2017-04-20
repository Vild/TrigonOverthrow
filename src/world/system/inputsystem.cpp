// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "inputsystem.hpp"

#include "../component/kbmouseinputcomponent.hpp"
#include "../component/physicscomponent.hpp"

#include "../../engine.hpp"

void InputSystem::update(World& world, float delta) {
	std::shared_ptr<HIDInput> hid = Engine::getInstance().getHIDInput();

	for (std::unique_ptr<Entity>& entity : world.getEntities()) {
		if (!entity->getComponent<KBMouseInputComponent>())
			continue;

		auto physicsComponent = entity->getComponent<PhysicsComponent>();
		if (!physicsComponent)
			continue;

		// TODO: calculate this?
		glm::vec3 forward = glm::vec3(0, 0, 1);
		glm::vec3 right = glm::vec3(-1, 0, 0);
		glm::vec3 up = glm::vec3(0, 1, 0);

		glm::vec3 inputDir;
		if (hid->getKey(SDL_SCANCODE_W))
			inputDir.z++;
		if (hid->getKey(SDL_SCANCODE_S))
			inputDir.z--;

		if (hid->getKey(SDL_SCANCODE_A))
			inputDir.x--;
		if (hid->getKey(SDL_SCANCODE_D))
			inputDir.x++;

		if (hid->getKey(SDL_SCANCODE_SPACE))
			inputDir.y++;
		if (hid->getKey(SDL_SCANCODE_LCTRL))
			inputDir.y--;

		float accelSpeed = 100;
		physicsComponent->acceleration = -physicsComponent->velocity / 0.05f;
		physicsComponent->acceleration += inputDir.z * forward * accelSpeed;
		physicsComponent->acceleration += inputDir.x * right * accelSpeed;
		physicsComponent->acceleration += inputDir.y * up * accelSpeed;
		
		//auto gun = entity->getComponent<GunComponent>();
		//if (hid->getKey(SDL_SCANCODE_F))
		//	gun->shoot;
	}
}

void InputSystem::registerImGui() {}
