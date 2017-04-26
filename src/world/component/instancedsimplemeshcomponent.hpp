#pragma once
#include <vector>
#include <memory>
#include "component.hpp"
#include "../../gl/simplemesh.hpp"
#include "transformcomponent.hpp"


class InstancedSimpleMeshComponent : public Component
{
public:
	InstancedSimpleMeshComponent(std::unique_ptr<SimpleMesh>& simpleMesh);
	virtual ~InstancedSimpleMeshComponent();

	void addInstance(TransformComponent* instance);

	void render();

	// Inherited via Component
	virtual std::string name() override;
	virtual void registerImGui() override;

private:
	std::unique_ptr<SimpleMesh> simpleMesh;
	std::vector<TransformComponent*> instances;

};