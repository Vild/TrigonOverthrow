#pragma once

#include <glm/glm.hpp>

#include "component.hpp"
#include "../../io/meshloader.hpp"

#include "../../lib/glad.h"

struct ModelComponent : public Component<ModelComponent> {
	std::shared_ptr<LoadedMesh> meshData;

	virtual void render(const glm::mat4& m, GLenum drawMode = GL_TRIANGLES);

	virtual std::string name() { return "ModelComponent"; }
};
