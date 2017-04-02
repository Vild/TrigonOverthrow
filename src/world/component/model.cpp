#include "model.hpp"

void ModelComponent::render(GLenum drawMode) {
	if (!meshData)
		return;

	if (meshData->texture)
		meshData->texture->bind(0);
	meshData->mesh->render(1, drawMode);
}
