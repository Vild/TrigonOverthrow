#pragma once
#include "component.hpp"
#include <glm/glm.hpp>

#include <vector>

// Note! When making changes here add them also to TransformComponent

struct FloorTransformComponent : public Component {
	int gridSize; // will be gridSize*gridSize
	glm::vec3 position = glm::vec3(0, 0, 0);
	glm::vec3 scale = glm::vec3(1, 1, 1);
	std::vector<glm::mat4> matrices;

	void recalculateMatrices();

	virtual void registerImGui();

	virtual std::string name() { return "FloorTransformComponent"; }
};
