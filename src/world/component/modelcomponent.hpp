#pragma once

#include "component.hpp"
#include "../../io/meshloader.hpp"

#include "../../lib/glad.h"

struct ModelComponent : public Component<ModelComponent> {
	std::shared_ptr<LoadedMesh> meshData;

	virtual void render(GLenum drawMode = GL_TRIANGLES);

	virtual std::string name() { return "ModelComponent"; }
};
