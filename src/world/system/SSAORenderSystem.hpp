#pragma once
#include <glm\glm.hpp>
#include "../../gl/gbuffer.hpp"
#include "../../gl/shader.hpp"

class SSAORenderSystem
{
public:
	typedef int Camera;

	SSAORenderSystem(int width, int height);


	GBuffer & render(GBuffer & prevBuffer, Camera & camera);

private:
	GBuffer gBuffer;
	ShaderProgram shaderProgram;
};