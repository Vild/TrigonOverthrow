// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "lightingrenderpass.hpp"

#include "../../engine.hpp"

#include "../component/transformcomponent.hpp"

LightingRenderPass::LightingRenderPass() {
	_gbuffer = std::make_shared<GBuffer>(0);

	_shader = std::make_shared<ShaderProgram>();
	_shader->attach(std::make_shared<ShaderUnit>("assets/shaders/final.vert", ShaderType::vertex))
		.attach(std::make_shared<ShaderUnit>("assets/shaders/final.frag", ShaderType::fragment))
		.finalize();
	_shader->bind()
		.addUniform("vp")
		.addUniform("defPos")
		.addUniform("defNormal")
		.addUniform("defDiffuseSpecular")
		.addUniform("defDepth")
		.addUniform("defOcclusionMap");
	_shader->setUniform("defPos", (GLint)InputAttachment::position)
		.setUniform("defNormal", (GLint)InputAttachment::normal)
		.setUniform("defDiffuseSpecular", (GLint)InputAttachment::diffuseSpecular)
		.setUniform("defDepth", (GLint)InputAttachment::depth)
		.setUniform("defOcclusionMap", (GLint)InputAttachment::OcclusionMap);

	std::vector<Vertex> vertices = {
		Vertex{glm::vec3{-1, 1, 0}, glm::vec3{0, 0, -1}, {1.0, 1.0, 1.0}, {0, 1}}, Vertex{glm::vec3{1, 1, 0}, glm::vec3{0, 0, -1}, {1.0, 1.0, 1.0}, {1, 1}},
		Vertex{glm::vec3{1, -1, 0}, glm::vec3{0, 0, -1}, {1.0, 1.0, 1.0}, {1, 0}}, Vertex{glm::vec3{-1, -1, 0}, glm::vec3{0, 0, -1}, {1.0, 1.0, 1.0}, {0, 0}},
	};
	std::vector<GLuint> indicies = {0, 2, 1, 2, 0, 3};
	_plane = std::make_shared<Mesh>(vertices, indicies);
	_plane
		->addBuffer("m",
								[](GLuint id) {
									glm::mat4 mData = glm::mat4(1);
									glBindBuffer(GL_ARRAY_BUFFER, id);
									glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), glm::value_ptr(mData), GL_STATIC_DRAW); // Will only be uploaded once

									for (int i = 0; i < 4; i++) {
										glEnableVertexAttribArray(ShaderAttributeID::m + i);
										glVertexAttribPointer(ShaderAttributeID::m + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4) * i));
										glVertexAttribDivisor(ShaderAttributeID::m + i, 1);
									}

									glBindBuffer(GL_ARRAY_BUFFER, 0);
								})
		.finalize();
}

LightingRenderPass::~LightingRenderPass() {}

void LightingRenderPass::render(World& world) {
	auto camera = Engine::getInstance().getCamera();
	if (!camera)
		return;

	auto transformComponent = camera->getComponent<TransformComponent>();
	if (!transformComponent)
		return;

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_shader->bind();

	_shader->setUniform("vp", glm::mat4(1));
	_plane->render();
}

void LightingRenderPass::resize(unsigned int width, unsigned int height) {
	_gbuffer->bind();
	glViewport(0, 0, width, height);
}

void LightingRenderPass::registerImGui() {}
