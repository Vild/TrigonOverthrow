#pragma once
#include <vector>
#include <memory>
#include "component.hpp"
#include "../../gl/simplemesh.hpp"
#include "transformcomponent.hpp"

class InstancedSimpleMeshComponent : public Component {
public:
	InstancedSimpleMeshComponent() = default;
	InstancedSimpleMeshComponent(std::unique_ptr<SimpleMesh>& simpleMesh);
	InstancedSimpleMeshComponent(const ComponentValues& value);
	virtual ~InstancedSimpleMeshComponent();

	void addInstance(TransformComponent* instance);

	void render();

	// Inherited via Component
	inline std::string name() final { return "InstancedSimpleMeshComponent"; }
	void registerImGui() final;

private:
	std::unique_ptr<SimpleMesh> simpleMesh;
	std::vector<TransformComponent*> instances;
};
