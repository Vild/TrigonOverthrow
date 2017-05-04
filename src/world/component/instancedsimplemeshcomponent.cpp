// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "instancedsimplemeshcomponent.hpp"
#include <algorithm>

InstancedSimpleMeshComponent::InstancedSimpleMeshComponent(std::unique_ptr<SimpleMesh>& simpleMesh) {
	this->simpleMesh = std::move(simpleMesh);
}

InstancedSimpleMeshComponent::~InstancedSimpleMeshComponent() {}

void InstancedSimpleMeshComponent::addInstance(TransformComponent* instance) {
	instances.push_back(instance);
}

void InstancedSimpleMeshComponent::render() {
	std::vector<glm::mat4> ms;
	for (auto& t : instances) {
		ms.push_back(t->getMatrix());
	}

	simpleMesh->draw(ms);
}

void InstancedSimpleMeshComponent::registerImGui() {}
