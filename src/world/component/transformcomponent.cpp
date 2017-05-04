// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "transformcomponent.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>

TransformComponent::TransformComponent() {
	dirty = false;
	position = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
	rotation = glm::quat();
	matrix = glm::mat4(1);
}

TransformComponent::~TransformComponent() {}

void TransformComponent::recalculateMatrix() {
	dirty = false;
	matrix = glm::translate(position) * glm::mat4_cast(rotation) * glm::scale(scale);
}

void TransformComponent::setPosition(const glm::vec3& position) {
	dirty |= this->position != position;
	this->position = position;
}

void TransformComponent::setRotation(const glm::quat& rotation) {
	dirty |= this->rotation != rotation;
	this->rotation = rotation;
}

void TransformComponent::setDirection(const glm::vec3& direction, const glm::vec3& up) {
	glm::vec3 upp = up;

	if (direction == up)
		upp.x += 0.0001;

	static const glm::vec3 O = {0, 0, 0};
	glm::mat3 m = glm::lookAt(O, direction, upp);
	setRotation(glm::quat_cast(m));
}

void TransformComponent::setScale(const glm::vec3& scale) {
	dirty |= this->scale != scale;
	;
	this->scale = scale;
}

void TransformComponent::move(const glm::vec3& delta) {
	setPosition(position + delta);
}

void TransformComponent::registerImGui() {
	dirty |= ImGui::DragFloat3("Position", glm::value_ptr(position), 0.1f);
	dirty |= ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.1f);
}
