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
	this->position = position;
	dirty = true;
}

void TransformComponent::setRotation(const glm::quat& rotation) {
	this->rotation = rotation;
	dirty = true;
}

void TransformComponent::setDirection(const glm::vec3& direction, const glm::vec3& up) {
	glm::vec3 upp = up;

	if (direction == up)
		upp.x += 0.0001;

	static const glm::vec3 O = {0, 0, 0};
	glm::mat3 m = glm::lookAt(O, direction, upp);
	rotation = glm::quat_cast(m);
	dirty = true;
}

void TransformComponent::setScale(const glm::vec3& scale) {
	this->scale = scale;
	dirty = true;
}

void TransformComponent::move(const glm::vec3& delta) {
	position += delta;
	dirty = true;
}

void TransformComponent::registerImGui() {
	// bool dirty = false;

	dirty |= ImGui::DragFloat3("Position", glm::value_ptr(position), 0.1f);
	dirty |= ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.1f);
}

