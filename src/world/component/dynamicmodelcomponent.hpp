#pragma once
#include "component.hpp"
#include "modelcomponent.hpp"
#include "glm/glm.hpp"

struct DynamicModelComponent : public Component {
	DynamicModelComponent();
	virtual ~DynamicModelComponent() {}
	void registerImGui();
	virtual std::string name() { return "DynamicModelComponent"; }

	std::map<int, std::shared_ptr<LoadedMesh>> meshes;
};