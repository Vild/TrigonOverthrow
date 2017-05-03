// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "inputsystem.hpp"

#include "../component/kbmouseinputcomponent.hpp"
#include "../component/physicscomponent.hpp"
#include "../component/rigidbodycomponent.hpp"
#include "../component/guncomponent.hpp"

#include "../../engine.hpp"

#include <stdio.h>

InputSystem::~InputSystem() {}

void InputSystem::update(World& world, float delta) {
	rmt_ScopedCPUSample(InputSystem, RMTSF_None);
	std::shared_ptr<HIDInput> hid = Engine::getInstance().getHIDInput();

	for (std::unique_ptr<Entity>& entity : world.getEntities()) {
		if (!entity->getComponent<KBMouseInputComponent>())
			continue;

		auto rigidBodyComponent = entity->getComponent<RigidBodyComponent>();
		if (!rigidBodyComponent)
			continue;

		btRigidBody & rigidBody = *rigidBodyComponent->getRigidBody();

		// TODO: calculate this?
		//glm::vec3 forward = glm::vec3(0, 0, 1);
		//glm::vec3 right = glm::vec3(-1, 0, 0);
		//glm::vec3 up = glm::vec3(0, 1, 0);

		glm::vec3 inputDir;
		if (hid->getKey(SDL_SCANCODE_W))
			inputDir.z++;
		if (hid->getKey(SDL_SCANCODE_S))
			inputDir.z--;

		if (hid->getKey(SDL_SCANCODE_A))
			inputDir.x++;
		if (hid->getKey(SDL_SCANCODE_D))
			inputDir.x--;

		if (hid->getKey(SDL_SCANCODE_SPACE))
			inputDir.y++;
		if (hid->getKey(SDL_SCANCODE_LCTRL))
			inputDir.y--;

		float accelSpeed = 60;
		float maxSpeed = 5;

		//physicsComponent->acceleration = -physicsComponent->velocity / 0.05f;
		//physicsComponent->acceleration += inputDir.z * forward * accelSpeed;
		//physicsComponent->acceleration += inputDir.x * right * accelSpeed;
		//physicsComponent->acceleration += inputDir.y * up * accelSpeed;

		
		rigidBody.applyCentralForce({
			inputDir.x * accelSpeed,
			inputDir.y * accelSpeed * 2,
			inputDir.z * accelSpeed
		});


		auto tc = entity->getComponent<TransformComponent>();
		if (tc)
		{
			static Engine * engine = &Engine::getInstance();
			int width = engine->getWidth();
			int height = engine->getHeight();

			glm::vec2 mpos = hid->getXY();
			glm::vec2 hmpos = { mpos.x / width, 1 - mpos.y / height };
			hmpos *= 2;
			hmpos -= 1;
		
			glm::vec2 dir = glm::normalize(hmpos);

			tc->setDirection({ dir.x, 0, dir.y });
		}


		auto gun = entity->getComponent<GunComponent>();
		if (gun && hid->getKey(SDL_SCANCODE_F) && gun->cooldown <= 0) {
			gun->shoot = true;
		}
	}
}

void InputSystem::registerImGui() {}
