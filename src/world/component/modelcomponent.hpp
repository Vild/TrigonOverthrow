#pragma once

#include <glm/glm.hpp>

#include "component.hpp"
#include "../../io/meshloader.hpp"

#include "../../lib/glad.h"

struct ModelComponent : public Component<ModelComponent> {
	std::shared_ptr<LoadedMesh> meshData;

	virtual void render(const glm::mat4& m, GLenum drawMode = GL_TRIANGLES);

	template <typename T>
	void render(const std::vector<T>& m, size_t count = 1, GLenum drawMode = GL_TRIANGLES) {
		meshData->texture->bind(0);
		meshData->normalTexture->bind(1);
		meshData->mesh->uploadBufferArray("m", m);
		meshData->mesh->render(count, drawMode);
	}

	virtual std::string name() { return "ModelComponent"; }
};
