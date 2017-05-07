// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "floortransformcomponent.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>

FloorTransformComponent::FloorTransformComponent(const ComponentValues& value) : FloorTransformComponent() {
	position = value.getVec3("position", {0, 0, 0});
	scale = value.getVec3("scale", {1, 1, 1});
}

FloorTransformComponent::~FloorTransformComponent() {}

void FloorTransformComponent::recalculateMatrices() {
	if (!matrices.size())
		matrices.resize(gridSize * gridSize);

	glm::mat4 matrix = glm::translate(position) * glm::scale(scale);

	for (int z = 0; z < gridSize; z++)
		for (int x = 0; x < gridSize; x++)
			matrices[z * gridSize + x] = glm::translate(glm::vec3(x, 0, z)) * matrix;
}

void FloorTransformComponent::registerImGui() {
	bool dirty = false;

	dirty |= ImGui::DragFloat3("Position", glm::value_ptr(position), 0.1);
	dirty |= ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.1);

	if (dirty)
		recalculateMatrices();
}
