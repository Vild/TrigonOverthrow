#include "SSAORenderSystem.hpp"
#include <random>

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

GBuffer & SSAORenderSystem::render(GBuffer & prevBuffer, Camera & camera)
{
	gBuffer.bind();
	auto colorBuffers = prevBuffer.getAttachments();

	shaderProgram.setUniform("positionMap", 0);
	shaderProgram.setUniform("normalMap", 1);
	
	return gBuffer;
}

float SSAORenderSystem::lerp(float a, float b, float f)
{
	return a + f * (b - a);
}

void SSAORenderSystem::generateUniformData(int width, int height)
{
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

		float scale = float(i) / 64.0;
		scale = lerp(0.1, 1.0, scale * scale);

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

	noiseMap = std::make_unique<Texture>(width, height, GL_RGB32F, GL_RGB, GL_FLOAT, &noiseData[0]);
	noiseMap->bind()
		.setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST)
		.setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST)
		.setParameter(GL_TEXTURE_WRAP_S, GL_REPEAT)
		.setParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
}

