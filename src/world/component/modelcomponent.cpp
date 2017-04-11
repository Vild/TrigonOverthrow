// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "modelcomponent.hpp"

void ModelComponent::render(const glm::mat4& m, GLenum drawMode) {
	meshData->texture->bind(0);
	meshData->normalTexture->bind(1);
	meshData->mesh->uploadBufferData("m", m);
	meshData->mesh->render(1, drawMode);
}

void ModelComponent::registerImGui() {}
