#include "instancedsimplemeshcomponent.hpp"
#include <algorithm>

InstancedSimpleMeshComponent::InstancedSimpleMeshComponent(std::unique_ptr<SimpleMesh> & simpleMesh)
{
	this->simpleMesh = std::move(simpleMesh);
}

InstancedSimpleMeshComponent::~InstancedSimpleMeshComponent()
{
}

void InstancedSimpleMeshComponent::addInstance(TransformComponent * instance)
{
	instances.push_back(instance);
}

void InstancedSimpleMeshComponent::render()
{
	std::vector<glm::mat4> ms;
	for (auto& t : instances)
	{
		ms.push_back(t->getMatrix());
	}

	simpleMesh->draw(ms);
}

inline std::string InstancedSimpleMeshComponent::name()
{
	return "InstancedSimpleMeshComponent";
}

void InstancedSimpleMeshComponent::registerImGui()
{
}
