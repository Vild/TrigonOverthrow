// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "modelcomponent.hpp"

#include "../../engine.hpp"

ModelComponent::ModelComponent(const ComponentValues& value) : ModelComponent() {
	std::string mesh, texture, normalTexture;
	mesh = value.getString("mesh", "");
	if (mesh.empty())
		throw "Failed to load mesh data!";

	Engine& engine = Engine::getInstance();

	meshData = engine.getMeshLoader()->getMesh(mesh);
	if (!meshData)
		throw "Failed to load mesh data!";

	texture = value.getString("texture", "");
	if (!texture.empty())
		meshData->texture = engine.getTextureManager()->getTexture(texture);

	normalTexture = value.getString("normalTexture", "");
	if (!normalTexture.empty())
		meshData->normalTexture = engine.getTextureManager()->getTexture(normalTexture);

	if (value.getBool("needsMMatrix", false))
		meshData->mesh->addBuffer("m", [](GLuint id) {
			glBindBuffer(GL_ARRAY_BUFFER, id);
			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);

			for (int i = 0; i < 4; i++) {
				glEnableVertexAttribArray(ShaderAttributeID::m + i);
				glVertexAttribPointer(ShaderAttributeID::m + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4) * i));
				glVertexAttribDivisor(ShaderAttributeID::m + i, 1);
			}

			glBindBuffer(GL_ARRAY_BUFFER, 0);
		});

	meshData->mesh->finalize();
}

ModelComponent::~ModelComponent() {}

void ModelComponent::render(const glm::mat4& m, GLenum drawMode) {
	meshData->texture->bind(0);
	meshData->normalTexture->bind(1);
	meshData->mesh->uploadBufferData("m", m);
	meshData->mesh->render(1, drawMode);
}

void ModelComponent::registerImGui() {}
