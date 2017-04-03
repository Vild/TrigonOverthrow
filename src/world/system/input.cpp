#include "input.hpp"

#include "../component/kbmouseinput.hpp"
#include "../component/transform.hpp"

void InputSystem::update(World& world, float delta) {
	for (std::shared_ptr<Entity> entity : world.getEntities()) {
		auto input = entity->getComponent<KBMouseInputComponent>();
		if (!input)
			continue;

		auto transform = entity->getComponent<TransformComponent>();
		if (!transform)
			continue;

		input->update(); // TODO: fix

		/*float mspeed = 0.005f;
		_yaw += mspeed * input->xyDiff.x;
		_pitch += mspeed * input->xyDiff.y;
		_pitch = glm::clamp(_pitch, (float)-M_PI / 2, (float)M_PI / 2);*/

		glm::vec3 forward = glm::vec3(0, 0, 1); // glm::vec3(cos(_pitch) * sin(_yaw), sin(_pitch), cos(_pitch) * cos(_yaw));
		glm::vec3 right = glm::vec3(-1, 0, 0);	// = glm::vec3(sin(_yaw - M_PI / 2.0f), 0, cos(_yaw - M_PI / 2.0f));
		glm::vec3 up = glm::vec3(0, 1, 0);			// glm::cross(right, forward);

		float accelSpeed = 1000;

		transform->acceleration = -transform->velocity / 0.05f;
		transform->acceleration += input->direction.z * forward * delta * accelSpeed;
		transform->acceleration += input->direction.x * right * delta * accelSpeed;
		transform->acceleration += input->direction.y * up * delta * accelSpeed;
	}
}