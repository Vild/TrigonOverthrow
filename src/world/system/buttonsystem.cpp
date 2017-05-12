// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "buttonsystem.hpp"

#include "../component/transformcomponent.hpp"
#include "../component/buttoncomponent.hpp"
#include "../component/cameracomponent.hpp"

#include "../../engine.hpp"

#include <iostream>

ButtonSystem::~ButtonSystem() {}

void ButtonSystem::update(World& world, float delta) {
	Engine& engine = Engine::getInstance();
	auto camera = engine.getCamera();
	if (!camera)
		return;

	auto cameraComponent = camera->getComponent<CameraComponent>();
	if (!cameraComponent)
		return;

	std::shared_ptr<HIDInput> hid = engine.getHIDInput();

	const glm::ivec2 xy = hid->getXY();
	unsigned int w = engine.getWidth(), h = engine.getHeight();

	const glm::vec4 rayStartNDC = glm::vec4((2.0 * xy.x) / w - 1.0, (2.0 * xy.y) / h - 1.0, -1, 1);
	const glm::vec4 rayEndNDC = glm::vec4((2.0 * xy.x) / w - 1.0, (2.0 * xy.y) / h - 1.0, 0, 1);
	const glm::mat4 invM = glm::inverse(cameraComponent->projectionMatrix * cameraComponent->viewMatrix);

	glm::vec4 rayStartWorld = invM * rayStartNDC;
	glm::vec4 rayEndWorld = invM * rayEndNDC;
	rayStartWorld /= rayStartWorld.w;
	rayEndWorld /= rayEndWorld.w;

	const glm::vec3 origin = glm::vec3(rayStartWorld);
	const glm::vec3 dir = glm::normalize(glm::vec3(rayEndWorld - rayStartWorld));

	Entity* entityHit = nullptr;
	float distanceHit = std::numeric_limits<float>::infinity();

	// XXX: HACK HACK
	ButtonComponent::ButtonCallback hackCB = nullptr;

	for (Entity * entity : world.getActiveComponents<ButtonComponent>()) {
			auto button = entity->getComponent<ButtonComponent>();
		if (!button)
			continue;

		auto transform = entity->getComponent<TransformComponent>();
		if (!transform)
			continue;

		// XXX: HACK HACK
		if (button->callback)
			hackCB = button->callback;

		glm::vec3 minPos = button->position;
		glm::vec3 maxPos = button->position + button->size;

		float tmin = -std::numeric_limits<float>::infinity();
		float tmax = std::numeric_limits<float>::infinity();

		glm::vec3 toCenter = glm::vec3(transform->getMatrix()[3]) - origin;

		glm::mat3 axies = glm::mat3(transform->getMatrix());

		float t;

		for (int i = 0; i < 3; i++) {
			float e = glm::dot(axies[i], toCenter);
			float f = glm::dot(axies[i], dir);

			if (abs(f) > _epsilon) {
				float t1 = (e + minPos[i]) / f;
				float t2 = (e + maxPos[i]) / f;
				if (t1 > t2)
					std::swap(t1, t2);

				if (t1 > tmin)
					tmin = t1;

				if (t2 < tmax)
					tmax = t2;

				if (tmin > tmax)
					goto breakLoop;
			} else if (-e + minPos[i] > 0 || -e + maxPos[i] < 0)
				goto breakLoop;
		}

		t = (tmin > 0) ? tmin : tmax;

		if (t < 0) {
		breakLoop:
			continue;
		}

		printf("Is over: %s t: %f\n", entity->getName().c_str(), t);
		if (t < distanceHit) {
			distanceHit = t;
			entityHit = entity;
		}
	}

	if (!entityHit) {
		// XXX: HACK HACK HACK
		if (!hackCB)
			return;
		return hackCB(nullptr, Engine::getInstance().getState(), hid->getMouseState());
	}
	auto button = entityHit->getComponent<ButtonComponent>();
	if (!button->callback)
		return;
	button->callback(entityHit, Engine::getInstance().getState(), hid->getMouseState());
}

void ButtonSystem::registerImGui() {}
