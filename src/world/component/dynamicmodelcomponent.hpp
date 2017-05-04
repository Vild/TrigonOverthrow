#pragma once
#include "component.hpp"
#include "modelcomponent.hpp"
#include "glm/glm.hpp"

struct DynamicModelComponent : public Component {
	DynamicModelComponent();
	virtual ~DynamicModelComponent() {}
	void registerImGui() final;
	inline std::string name() final { return "DynamicModelComponent"; }

	std::map<int, std::shared_ptr<LoadedMesh>> meshes;
};
