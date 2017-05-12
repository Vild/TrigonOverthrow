// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "ssaorenderpass.hpp"
#include "../../engine.hpp"
#include <random>
#include "../component/cameracomponent.hpp"

SSAORenderSystem::SSAORenderSystem() {
	sampleSize = 16;
	sampleRadius = 0.75f;
	sampleBias = 0.01f;

	auto& engine = Engine::getInstance();
	int width = engine.getWidth();
	int height = engine.getHeight();

	_shader = std::make_shared<ShaderProgram>();
	_shader->attach("assets/shaders/ssao.vert", ShaderType::vertex).attach("assets/shaders/ssao.frag", ShaderType::fragment).finalize();

	_shader->bind()
		.addUniform("positionMap")
		.addUniform("normalMap")
		.addUniform("noiseMap")
		.addUniform("noiseScale")
		.addUniform("sampleSize")
		.addUniform("sampleRadius")
		.addUniform("sampleBias")
		.addUniform("samplePoints")
		.addUniform("viewMatrix")
		.addUniform("projectionMatrix");

	_gbuffer = std::make_shared<GBuffer>();
	_gbuffer->bind().attachTexture(Attachments::occlusionMap, width, height, GL_RED, GL_FLOAT, 1).finalize();

	//_gbuffer->getAttachments()[0]->bind()
	//	.setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR)
	//	.setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	std::vector<Vertex> vertices = {
		Vertex{glm::vec3{-1, 1, 0}, glm::vec3{0, 0, -1}, {1.0, 1.0, 1.0}, {0, 1}},	//
		Vertex{glm::vec3{1, 1, 0}, glm::vec3{0, 0, -1}, {1.0, 1.0, 1.0}, {1, 1}},		//
		Vertex{glm::vec3{1, -1, 0}, glm::vec3{0, 0, -1}, {1.0, 1.0, 1.0}, {1, 0}},	//
		Vertex{glm::vec3{-1, -1, 0}, glm::vec3{0, 0, -1}, {1.0, 1.0, 1.0}, {0, 0}}, //
	};
	std::vector<GLuint> indicies = {0, 2, 1, 2, 0, 3};
	
	fsquad = std::make_unique<SimpleMesh>(GL_TRIANGLE_STRIP, SimpleMesh::quad);

	generateUniformData(width, height);
}

SSAORenderSystem::~SSAORenderSystem() {}

float SSAORenderSystem::lerp(float a, float b, float f) {
	return a + f * (b - a);
}

void SSAORenderSystem::generateUniformData(int width, int height) {
	std::uniform_real_distribution<GLfloat> randomFlaots(0.0, 1.0);
	std::default_random_engine generator;

	std::vector<glm::vec3> samplePoints;
	for (size_t i = 0; i < 64; i++) {

		glm::vec3 samplePoint;
		float dot;
		do {
			samplePoint = {
				randomFlaots(generator) * 2.0 - 1.0,
				randomFlaots(generator) * 2.0 - 1.0,
				randomFlaots(generator),
			};

			dot = glm::dot(samplePoint, { 0,1,0 });
		} while (dot < 0.25);
		

		samplePoint = glm::normalize(samplePoint);
		samplePoint *= randomFlaots(generator);

		float scale = float(i) / 64.0f;
		scale = lerp(0.1f, 1.0f, scale * scale);

		samplePoint *= scale;

		samplePoints.push_back(samplePoint);
	}

	std::vector<glm::vec3> noiseData;
	for (size_t i = 0; i < 64; i++) {
		glm::vec3 noise = {
			0, 
			1,
			0,
		};

		noiseData.push_back(noise);
	}

	noiseMap = std::make_shared<Texture>(8, 8, GL_RGB16F, GL_RGB, GL_FLOAT, &noiseData[0]);
	noiseMap->bind()
		.setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST)
		.setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST)
		.setParameter(GL_TEXTURE_WRAP_S, GL_REPEAT)
		.setParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);

	attachInputTexture(InputAttachments::noiseMap_INTERNAL, noiseMap);

	glm::vec2 noiseScale = {width / 8.0, height / 8.0};

	_shader->setUniform("positionMap", (GLint)InputAttachments::positionMap);
	_shader->setUniform("normalMap", (GLint)InputAttachments::normalMap);
	_shader->setUniform("noiseMap", (GLint)InputAttachments::noiseMap_INTERNAL);

	_shader->setUniform("noiseScale", noiseScale);

	_shader->setUniform("sampleSize", sampleSize);
	_shader->setUniform("sampleRadius", sampleRadius);
	_shader->setUniform("sampleBias", sampleBias);
	_shader->setUniformArray("samplePoints", samplePoints);
}

void SSAORenderSystem::resize(unsigned int width, unsigned int height) {
	auto& attachments = _gbuffer->getAttachments();
	attachments[Attachments::occlusionMap]->resize(width, height, GL_RED, GL_RED, GL_FLOAT);

	_gbuffer->bind();
	glViewport(0, 0, width, height);
}

void SSAORenderSystem::registerImGui() {
	bool dirty = false;

	dirty |= ImGui::DragInt("Sample Size", &sampleSize, 1, 0, 64);
	dirty |= ImGui::DragFloat("Sample Radius", &sampleRadius, 0.01f);
	dirty |= ImGui::DragFloat("Sample Bias", &sampleBias, 0.001f);

	if (dirty)
		_shader->bind().setUniform("sampleSize", sampleSize).setUniform("sampleRadius", sampleRadius).setUniform("sampleBias", sampleBias);
}

void SSAORenderSystem::render(World& world) {
	glClear(GL_COLOR_BUFFER_BIT);
	auto camera = Engine::getInstance().getCamera();
	auto cameraComponent = camera->getComponent<CameraComponent>();
	if (!cameraComponent)
		return;

	_shader->bind();
	_shader->setUniform("viewMatrix", cameraComponent->viewMatrix);
	_shader->setUniform("projectionMatrix", cameraComponent->projectionMatrix);

	fsquad->draw();
}
