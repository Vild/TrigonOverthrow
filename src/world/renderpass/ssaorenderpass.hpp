#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "../../gl/gbuffer.hpp"
#include "../../gl/shader.hpp"
#include "../../gl/texture.hpp"
#include "renderpass.hpp"

class SSAORenderSystem : public RenderPass
{
public:
	typedef int Camera;

	SSAORenderSystem(int width, int height);

	GBuffer & render(GBuffer & prevBuffer, Camera & camera);

	// Inherited via RenderPass
	virtual void render(World & world) override;

private:
	GBuffer gBuffer;
	ShaderProgram shaderProgram;

	std::shared_ptr<Texture> noiseMap;

	float lerp(float a, float b, float c);
	void generateUniformData(int width, int height);


};