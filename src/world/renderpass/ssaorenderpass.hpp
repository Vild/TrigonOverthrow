#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "../../gl/gbuffer.hpp"
#include "../../gl/shader.hpp"
#include "../../gl/texture.hpp"
#include "../../gl/mesh.hpp"
#include "renderpass.hpp"

class SSAORenderSystem : public RenderPass
{
public:
	enum InputAttachments { PositionMap, NormalMap, NoiseMap };
	enum Attachments { OcclusionMap };
	typedef int Camera;

	SSAORenderSystem();
	
	// Inherited via RenderPass
	virtual void render(World & world) override;
	virtual void resize(unsigned int width, unsigned int height) override;

	virtual void registerImGui();
	inline virtual std::string name() { return "SSAORenderPass"; };
private:
	std::shared_ptr<Texture> noiseMap;
	std::unique_ptr<Mesh> _plane;


	GLint sampleSize;
	GLfloat sampleRadius;
	GLfloat sampleBias;


	float lerp(float a, float b, float c);
	void generateUniformData(int width, int height);
};