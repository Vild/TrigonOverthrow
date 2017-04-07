#include "inputsystem.hpp"

#include "../component/kbmouseinputcomponent.hpp"
#include "../component/physicscomponent.hpp"

#include "../../engine.hpp"

void InputSystem::update(World& world, float delta) {
	std::shared_ptr<HIDInput> hid = Engine::getInstance().getHIDInput();

	for (std::shared_ptr<Entity> entity : world.getEntities()) {
		if (!entity->getComponent<KBMouseInputComponent>())
			continue;

		auto physicsComponent = entity->getComponent<PhysicsComponent>();
		if (!physicsComponent)
			continue;

		/*float mspeed = 0.005f;
		_yaw += mspeed * hid->getXYDiff().x;
		_pitch += mspeed * hid->getXYDiff().y;
		_pitch = glm::clamp(_pitch, (float)-M_PI / 2, (float)M_PI / 2);*/

		glm::vec3 forward = glm::vec3(0, 0, 1); // glm::vec3(cos(_pitch) * sin(_yaw), sin(_pitch), cos(_pitch) * cos(_yaw));
		glm::vec3 right = glm::vec3(-1, 0, 0);	// = glm::vec3(sin(_yaw - M_PI / 2.0f), 0, cos(_yaw - M_PI / 2.0f));
		glm::vec3 up = glm::vec3(0, 1, 0);			// glm::cross(right, forward);

		float accelSpeed = 1500;

		glm::vec3 inputDir = hid->getDirection();
		physicsComponent->acceleration = -physicsComponent->velocity / 0.05f;
		physicsComponent->acceleration += inputDir.z * forward * delta * accelSpeed;
		physicsComponent->acceleration += inputDir.x * right * delta * accelSpeed;
		physicsComponent->acceleration += inputDir.y * up * delta * accelSpeed;
	}
}
