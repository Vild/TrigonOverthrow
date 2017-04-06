#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "../../gl/gbuffer.hpp"
#include "../../gl/shader.hpp"
#include "../../gl/texture.hpp"

class SSAORenderSystem
{
public:
	typedef int Camera;

	SSAORenderSystem(int width, int height);


	GBuffer & render(GBuffer & prevBuffer, Camera & camera);

private:
	GBuffer gBuffer;
	ShaderProgram shaderProgram;

	Texture noiseMap;

	float lerp(float a, float b, float c);
	void generateUniformData();
};