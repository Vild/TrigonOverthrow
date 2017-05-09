// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "lightingrenderpass.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../../engine.hpp"
#include "../../lib/imgui.h"

#include "../component/transformcomponent.hpp"
#include "../component/suncomponent.hpp"
#include "../component/pointlightcomponent.hpp"

LightingRenderPass::LightingRenderPass() {
	_ambient = glm::vec3(0.1, 0.1, 0.1);
	_dirLight = DirLight{
		glm::vec3(1, 1, 1),				//
		glm::vec3(0.5, 0.5, 0.5), //
		glm::vec3(0, -1, 0)				//
	};

	for (int i = 0; i < MAX_POINT_LIGHTS; i++) {
		_pointLights[i] = PointLight{
			glm::vec3{(i % 8) / 8.0, (i % 16) / 16.0, (i % 2) / 2.0}, //
			glm::vec3{0.0, 0.0, 0.0},																	//

			glm::vec3{i, 1, i}, //
			float{1.0f},				//
			float{0.7f},				//
			float{1.8f},				//
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

		.addUniform("settings_enableDirLight")
		.addUniform("settings_enablePointLight")
		.addUniform("settings_shininess")

		.addUniform("cameraPos")
		.addUniform("ambient")

		.addUniform("dirLight.diffuse")
		.addUniform("dirLight.specular")
		.addUniform("dirLight.direction")

		.addUniform("pointLightCount");

	for (int i = 0; i < MAX_POINT_LIGHTS; i++) {
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
		.setUniform("defOcclusionMap", (GLint)InputAttachment::occlusionMap)

		.setUniform("settings_enableDirLight", _settings_enableDirLight)
		.setUniform("settings_enablePointLight", _settings_enablePointLight)
		.setUniform("settings_shininess", 0.5f);

	_shader->setUniform("ambient", _ambient)
		.setUniform("dirLight.diffuse", _dirLight.diffuse)
		.setUniform("dirLight.specular", _dirLight.specular)
		.setUniform("dirLight.direction", _dirLight.direction);

	for (int i = 0; i < MAX_POINT_LIGHTS; i++) {
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
	auto camera = Engine::getInstance().getCamera();
	if (!camera)
		return;

	auto transformComponent = camera->getComponent<TransformComponent>();
	if (!transformComponent)
		return;

	glClearColor(.9, .3, .9, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	auto camPos = transformComponent->getPosition();
	_shader->bind().setUniform("cameraPos", camPos);

	bool enableDirLight = false;
	bool enablePointLight = false;
	int pointLightCount = 0;
	float distances[MAX_POINT_LIGHTS];

	for (std::unique_ptr<Entity>& entity : world.getEntities()) {
		auto sun = entity->getComponent<SunComponent>();
		if (sun) {
			if (_ambient != sun->ambient) {
				_ambient = sun->ambient;
				_shader->setUniform("ambient", _ambient);
			}
			if (_dirLight != sun->directionLight) {
				_dirLight = sun->directionLight;
				_shader->setUniform("dirLight.diffuse", _dirLight.diffuse)
					.setUniform("dirLight.specular", _dirLight.specular)
					.setUniform("dirLight.direction", glm::normalize(_dirLight.direction));
			}
			enableDirLight = true;
		} else {
			auto pointLight = entity->getComponent<PointLightComponent>();
			if (!pointLight)
				continue;
			auto transform = entity->getComponent<TransformComponent>();
			if (!transform)
				continue;
			// XXX: Hack make nicer
			pointLight->pointLight.position = transform->getPosition() + pointLight->offset;

			int idx = pointLightCount;
			auto myDist = glm::distance(pointLight->pointLight.position, camPos);
			if (idx >= MAX_POINT_LIGHTS) {
				float biggestDist = 0;
				int biggestIdx = 0;
				for (int i = 0; i < MAX_POINT_LIGHTS; i++) {
					auto dist = glm::distance(_pointLights[i].position, camPos);
					if (biggestDist < dist) {
						biggestDist = dist;
						biggestIdx = i;
					}
				}

				if (myDist > biggestDist)
					continue;

				idx = biggestIdx;
			} else
				pointLightCount++;
			auto& light = _pointLights[idx];

			distances[idx] = myDist;

			if (light != pointLight->pointLight) {
				light = pointLight->pointLight;

				std::string pointStr = "pointLights[" + std::to_string(idx) + "]";

				_shader->setUniform(pointStr + ".diffuse", light.diffuse)
					.setUniform(pointStr + ".specular", light.specular)

					.setUniform(pointStr + ".position", light.position)
					.setUniform(pointStr + ".constant", light.constant)
					.setUniform(pointStr + ".linear", light.linear)
					.setUniform(pointStr + ".quadratic", light.quadratic);
			}
		}
	}
	enablePointLight = !!pointLightCount;

	_shader->setUniform("settings_enableDirLight", enableDirLight && _settings_enableDirLight)
		.setUniform("settings_enablePointLight", enablePointLight && _settings_enablePointLight)
		.setUniform("pointLightCount", pointLightCount);

	_shader->setUniform("vp", glm::mat4(1));
	_plane->render();
}

void LightingRenderPass::resize(unsigned int width, unsigned int height) {
	_gbuffer->bind();
	glViewport(0, 0, width, height);
}

void LightingRenderPass::registerImGui() {
	_shader->bind();
	if (ImGui::Checkbox("Directional Light", &_settings_enableDirLight))
		_shader->setUniform("settings_enableDirLight", _settings_enableDirLight);

	if (ImGui::Checkbox("Point Lights", &_settings_enablePointLight))
		_shader->setUniform("settings_enablePointLight", _settings_enablePointLight);
}
