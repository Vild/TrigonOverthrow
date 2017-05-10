// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "inputsystem.hpp"

#include "../component/kbmouseinputcomponent.hpp"
#include "../component/rigidbodycomponent.hpp"
#include "../component/guncomponent.hpp"
#include "../component/particlecomponent.hpp"

#include "../../engine.hpp"

#include <cstdio>

InputSystem::~InputSystem() {}

void InputSystem::update(World& world, float delta) {
	std::shared_ptr<HIDInput> hid = Engine::getInstance().getHIDInput();

	for (Entity * entity : Entity::getEntities<KBMouseInputComponent>()) {
		auto rigidBodyComponent = entity->getComponent<RigidBodyComponent>();
		if (!rigidBodyComponent)
			continue;

		btRigidBody& rigidBody = *rigidBodyComponent->getRigidBody();

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

		float accelSpeed = 20;
		// float maxSpeed = 5;

		rigidBody.applyCentralForce({inputDir.x * accelSpeed, inputDir.y * accelSpeed * 2, inputDir.z * accelSpeed});

		auto tc = entity->getComponent<TransformComponent>();
		if (tc) {
			static Engine* engine = &Engine::getInstance();
			int width = engine->getWidth();
			int height = engine->getHeight();

			glm::vec2 mpos = hid->getXY();
			glm::vec2 hmpos = {mpos.x / width, 1 - mpos.y / height};
			hmpos *= 2;
			hmpos -= 1;

			glm::vec2 dir = glm::normalize(hmpos);

			tc->setDirection({dir.x, 0, dir.y});
		}

		auto gun = entity->getComponent<GunComponent>();
		if (gun && hid->getKey(SDL_SCANCODE_F) && gun->cooldown <= 0) {
			gun->shoot = true;
		}
	}
}

void InputSystem::registerImGui() {}
