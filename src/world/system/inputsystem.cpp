// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "inputsystem.hpp"

#include "../component/kbmouseinputcomponent.hpp"
#include "../component/rigidbodycomponent.hpp"
#include "../component/guncomponent.hpp"
#include "../component/particlecomponent.hpp"
#include "../component/cameracomponent.hpp"
#include "../component/lookatcomponent.hpp"

#include "../../engine.hpp"
#include "../system/roomloadingsystem.hpp"

#include <cstdio>

InputSystem::~InputSystem() {}

void InputSystem::update(World& world, float delta) {
	std::shared_ptr<HIDInput> hid = Engine::getInstance().getHIDInput();

	for (Entity * entity : world.getActiveComponents<KBMouseInputComponent>()) {

		glm::vec3 inputDir;
		if (hid->getKey(SDL_SCANCODE_W))
			inputDir.z++;
		if (hid->getKey(SDL_SCANCODE_S))
			inputDir.z--;

		if (hid->getKey(SDL_SCANCODE_A))
			inputDir.x++;
		if (hid->getKey(SDL_SCANCODE_D))
			inputDir.x--;

		if (hid->getKey(SDL_SCANCODE_LSHIFT))
			inputDir.y++;
		if (hid->getKey(SDL_SCANCODE_LCTRL))
			inputDir.y--;

		if (glm::length(inputDir) > 0)
			inputDir = glm::normalize(inputDir);

		auto tc = entity->getComponent<TransformComponent>();
		tc->move(inputDir * delta * 10.f);
	}

	for (Entity * entity : world.getActiveComponents<CameraComponent>()) 
	{
		auto lookat = entity->getComponent<LookAtComponent>();
		glm::vec3 inputDir;
		if (hid->getKey(SDL_SCANCODE_UP))
			inputDir.z++;
		if (hid->getKey(SDL_SCANCODE_DOWN))
			inputDir.z--;

		if (hid->getKey(SDL_SCANCODE_LEFT))
			inputDir.x++;
		if (hid->getKey(SDL_SCANCODE_RIGHT))
			inputDir.x--;

		if (hid->getKey(SDL_SCANCODE_RSHIFT))
			inputDir.y++;
		if (hid->getKey(SDL_SCANCODE_RCTRL))
			inputDir.y--;

		lookat->offsetFromTarget += inputDir * delta;
	}

}

void InputSystem::registerImGui() {}
