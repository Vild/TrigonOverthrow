#include "modelcomponent.hpp"

void ModelComponent::render(const glm::mat4& m, GLenum drawMode) {
	if (!meshData)
		return;

	if (meshData->texture)
		meshData->texture->bind(0);
	meshData->mesh->uploadBufferData("m", m);
	meshData->mesh->render(1, drawMode);
}
