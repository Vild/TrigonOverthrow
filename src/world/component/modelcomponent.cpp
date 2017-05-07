// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "modelcomponent.hpp"

#include "../../engine.hpp"

ModelComponent::ModelComponent(const ComponentValues& value) : ModelComponent() {
	std::string mesh, texture, normalTexture;
	mesh = value.getString("mesh", "");
	if (mesh.empty())
		return;

	Engine& engine = Engine::getInstance();

	meshData = engine.getMeshLoader()->getMesh(mesh);

	texture = value.getString("texture", "");
	if (!texture.empty())
		meshData->texture = engine.getTextureManager()->getTexture(texture);

	normalTexture = value.getString("normalTexture", "");
	if (!normalTexture.empty())
		meshData->normalTexture = engine.getTextureManager()->getTexture(normalTexture);
}

ModelComponent::~ModelComponent() {}

void ModelComponent::render(const glm::mat4& m, GLenum drawMode) {
	meshData->texture->bind(0);
	meshData->normalTexture->bind(1);
	meshData->mesh->uploadBufferData("m", m);
	meshData->mesh->render(1, drawMode);
}

void ModelComponent::registerImGui() {}
