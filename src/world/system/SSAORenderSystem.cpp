#include "SSAORenderSystem.hpp"

SSAORenderSystem::SSAORenderSystem(int width, int height)
{
	shaderProgram
		.attach("", ShaderType::vertex)
		.attach("", ShaderType::fragment)
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


}

GBuffer & SSAORenderSystem::render(GBuffer & prevBuffer, Camera & camera)
{
	gBuffer.bind();
	auto colorBuffers = prevBuffer.getAttachments();

	shaderProgram.setUniform("positionMap", colorBuffers.at(0));
	shaderProgram.setUniform("normalMap", colorBuffers.at(1));
	
	return gBuffer;
}