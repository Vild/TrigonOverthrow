// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "lightingrenderpass.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../../engine.hpp"
#include "../../lib/imgui.h"

#include "../component/transformcomponent.hpp"

LightingRenderPass::LightingRenderPass() {
	_ambient = glm::vec3(0.1, 0.1, 0.1);
	_dirLight = DirLight{
		.diffuse = glm::vec3(1, 1, 1),				//
		.specular = glm::vec3(0.5, 0.5, 0.5), //
		.direction = glm::vec3(0, -1, 0)			//
	};

	for (int i = 0; i < POINT_LIGHTS; i++) {
		_pointLights[i] = PointLight{
			.diffuse = glm::vec3{(i % 8) / 8.0, (i % 16) / 16.0, (i % 2) / 2.0}, //
			.specular = glm::vec3{0.0, 0.0, 0.0},																 //

			.position = glm::vec3{i, 1, i}, //
			.constant = float{1},					 //
			.linear = float{0.7},					 //
			.quadratic = float{1.8},			 //
		};
	}

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
		.addUniform("defOcclusionMap")
		.addUniform("cameraPos")
		.addUniform("ambient")
		.addUniform("dirLight.ambient")
		.addUniform("dirLight.diffuse")
		.addUniform("dirLight.specular")
		.addUniform("dirLight.direction");

	for (int i = 0; i < POINT_LIGHTS; i++) {
		_shader->addUniform("pointLights[" + std::to_string(i) + "].diffuse")
			.addUniform("pointLights[" + std::to_string(i) + "].specular")

			.addUniform("pointLights[" + std::to_string(i) + "].position")
			.addUniform("pointLights[" + std::to_string(i) + "].constant")
			.addUniform("pointLights[" + std::to_string(i) + "].linear")
			.addUniform("pointLights[" + std::to_string(i) + "].quadratic");
	}

	_shader->setUniform("defPos", (GLint)InputAttachment::position)
		.setUniform("defNormal", (GLint)InputAttachment::normal)
		.setUniform("defDiffuseSpecular", (GLint)InputAttachment::diffuseSpecular)
		.setUniform("defDepth", (GLint)InputAttachment::depth)
		.setUniform("defOcclusionMap", (GLint)InputAttachment::OcclusionMap)
		.setUniform("ambient", _ambient)
		.setUniform("dirLight.diffuse", _dirLight.diffuse)
		.setUniform("dirLight.specular", _dirLight.specular)
		.setUniform("dirLight.direction", _dirLight.direction);

	for (int i = 0; i < POINT_LIGHTS; i++) {
		_shader->setUniform("pointLights[" + std::to_string(i) + "].diffuse", _pointLights[i].diffuse)
			.setUniform("pointLights[" + std::to_string(i) + "].specular", _pointLights[i].specular)

			.setUniform("pointLights[" + std::to_string(i) + "].position", _pointLights[i].position)
			.setUniform("pointLights[" + std::to_string(i) + "].constant", _pointLights[i].constant)
			.setUniform("pointLights[" + std::to_string(i) + "].linear", _pointLights[i].linear)
			.setUniform("pointLights[" + std::to_string(i) + "].quadratic", _pointLights[i].quadratic);
	}

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
	rmt_ScopedCPUSample(LightingRenderPass, RMTSF_None);
	rmt_ScopedOpenGLSample(LightingRenderPass);
	auto camera = Engine::getInstance().getCamera();
	if (!camera)
		return;

	auto transformComponent = camera->getComponent<TransformComponent>();
	if (!transformComponent)
		return;

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_shader->bind().setUniform("cameraPos", transformComponent->getPosition());

	_shader->setUniform("vp", glm::mat4(1));
	_plane->render();
}

void LightingRenderPass::resize(unsigned int width, unsigned int height) {
	_gbuffer->bind();
	glViewport(0, 0, width, height);
}

void LightingRenderPass::registerImGui() {
	_shader->bind();

	if (ImGui::ColorEdit3("Ambient", glm::value_ptr(_ambient)))
		_shader->setUniform("ambient", _ambient);

	ImGui::Text("Directional Light");
	if (ImGui::ColorEdit3("Diffuse", glm::value_ptr(_dirLight.diffuse)))
		_shader->setUniform("dirLight.diffuse", _dirLight.diffuse);
	if (ImGui::ColorEdit3("Specular", glm::value_ptr(_dirLight.specular)))
		_shader->setUniform("dirLight.specular", _dirLight.specular);
	if (ImGui::DragFloat3("Direction", glm::value_ptr(_dirLight.direction), 0.1))
		_shader->setUniform("dirLight.direction", _dirLight.direction);

	for (int i = 0; i < POINT_LIGHTS; i++) {
		auto& light = _pointLights[i];
		const ImVec4 color = ImColor(light.diffuse.x, light.diffuse.y, light.diffuse.z, 1.0);
		ImVec4 invert = color;
		invert.x = 1 - invert.x;
		invert.y = 1 - invert.y;
		invert.z = 1 - invert.z;

		std::string name = std::string("Light") + std::to_string(i);
		ImGui::PushStyleColor(ImGuiCol_Header, color);
		ImGui::PushStyleColor(ImGuiCol_Text, invert);
		if (ImGui::CollapsingHeader(name.c_str())) {
			ImGui::PopStyleColor(2);
			ImGui::Text((std::string("Editing Light #") + std::to_string(i)).c_str());

			if (ImGui::ColorEdit3("Diffuse##diffuse", glm::value_ptr(light.diffuse)))
				_shader->setUniform("pointLights[" + std::to_string(i) + "].diffuse", light.diffuse);
			if (ImGui::ColorEdit3("Specular##specular", glm::value_ptr(light.specular)))
				_shader->setUniform("pointLights[" + std::to_string(i) + "].specular", light.specular);
			if (ImGui::DragFloat3("Position##position", glm::value_ptr(light.position), 0.1))
				_shader->setUniform("pointLights[" + std::to_string(i) + "].position", light.position);
			if (ImGui::DragFloat("Constant##constant", &light.constant, 0.1))
				_shader->setUniform("pointLights[" + std::to_string(i) + "].constant", light.constant);
			if (ImGui::DragFloat("Linear##linear", &light.linear, 0.1))
				_shader->setUniform("pointLights[" + std::to_string(i) + "].linear", light.linear);
			if (ImGui::DragFloat("Quadratic##quadratic", &light.quadratic, 0.1))
				_shader->setUniform("pointLights[" + std::to_string(i) + "].quadratic", light.quadratic);
		} else
			ImGui::PopStyleColor(2);
	}
}
