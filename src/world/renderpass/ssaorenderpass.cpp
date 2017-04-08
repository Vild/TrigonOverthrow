#include "ssaorenderpass.hpp"
#include "../../engine.hpp"
#include <random>
#include "../component/cameracomponent.hpp"

SSAORenderSystem::SSAORenderSystem(int width, int height)
{
	shaderProgram
		.attach("assets/shaders/ssao.vert", ShaderType::vertex)
		.attach("assets/shaders/ssao.frag", ShaderType::fragment)
		.finalize();

	shaderProgram
		.addUniform("positionMap")
		.addUniform("normalMap")
		.addUniform("noiseMap")
		.addUniform("noiseScale")
		.addUniform("nrOfSamples")
		.addUniform("sampleRadius")
		.addUniform("sampleBias")
		.addUniform("samplePoints")
		.addUniform("viewMatrix")
		.addUniform("projectionMatrix");

	gBuffer.attachTexture(0, width, height, GL_RED, GL_FLOAT, 1);

	
	shaderProgram.bind();
	generateUniformData(width, height);
}

float SSAORenderSystem::lerp(float a, float b, float f)
{
	return a + f * (b - a);
}

void SSAORenderSystem::generateUniformData(int width, int height)
{
	shaderProgram.setUniform("positionMap", 0);
	shaderProgram.setUniform("normalMap", 1);
	shaderProgram.setUniform("normalMap", 2);

	std::uniform_real_distribution<GLfloat> randomFlaots(0.0, 1.0);
	std::default_random_engine generator;

	std::vector<glm::vec3> samplePoints;
	for (size_t i = 0; i < 64; i++)
	{
		glm::vec3 samplePoint = {
			randomFlaots(generator) * 2.0 - 1.0,
			randomFlaots(generator) * 2.0 - 1.0,
			randomFlaots(generator)
		};

		samplePoint = glm::normalize(samplePoint);
		samplePoint *= randomFlaots(generator);

		float scale = float(i) / 64.0f;
		scale = lerp(0.1f, 1.0f, scale * scale);

		samplePoint *= scale;

		samplePoints.push_back(samplePoint);
	}

	shaderProgram.setUniformArray("samplePoints", samplePoints);

	std::vector<glm::vec3> noiseData;
	for (size_t i = 0; i < 16; i++)
	{
		glm::vec3 noise = {
			randomFlaots(generator) * 2.0 - 1.0,
			randomFlaots(generator) * 2.0 - 1.0,
			0.0
		};

		noiseData.push_back(noise);
	}

	noiseMap = std::make_shared<Texture>(width, height, GL_RGB32F, GL_RGB, GL_FLOAT, &noiseData[0]);
	noiseMap->bind()
		.setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST)
		.setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST)
		.setParameter(GL_TEXTURE_WRAP_S, GL_REPEAT)
		.setParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);

	attachInputTexture(2, noiseMap);

	glm::vec2 noiseScale = { width / 4.0, height / 4.0 };
	shaderProgram.setUniform("noiseScale", noiseScale);

	shaderProgram.setUniform("sampleSize", 64);
	shaderProgram.setUniform("sampleRadius", 0.5f);
	shaderProgram.setUniform("sampleBias", 0.025f);
}

void SSAORenderSystem::fsQuadRender()
{
	static bool first = true;
	static GLuint VAO, VBO;

	if (first)
	{
		glm::vec3 vertices[4] = {
			{ -1,  1,  0 },
			{  1,  1,  0 },
			{ -1, -1,  0 },
			{  1, -1,  0 }
		};

		glGenBuffers(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

		glBindVertexArray(0);

		first = false;
	}

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void SSAORenderSystem::render(World & world)
{
	CameraEntity & camera = *Engine::getInstance().getCamera();
	auto cameraComponent = camera.getComponent<CameraComponent>();

	shaderProgram.bind();
	shaderProgram.setUniform("viewMatrix", cameraComponent->viewMatrix);
	shaderProgram.setUniform("projectionMatrix", cameraComponent->projectionMatrix);

	fsQuadRender();
}

