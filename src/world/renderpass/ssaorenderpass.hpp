#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "../../gl/gbuffer.hpp"
#include "../../gl/shader.hpp"
#include "../../gl/texture.hpp"
#include "../../gl/simplemesh.hpp"
#include "renderpass.hpp"

class SSAORenderSystem : public RenderPass {
public:
	enum InputAttachments : GLint { positionMap = 0, normalMap, noiseMap_INTERNAL };
	enum Attachments : GLint { occlusionMap = 0 };

	SSAORenderSystem();
	virtual ~SSAORenderSystem();

	// Inherited via RenderPass
	void render(World& world) final;
	void resize(unsigned int width, unsigned int height) final;

	void registerImGui() final;
	inline std::string name() final { return "SSAORenderPass"; };

private:
	std::shared_ptr<Texture> noiseMap;
	std::unique_ptr<SimpleMesh> fsquad;

	GLint sampleSize;
	GLfloat sampleRadius;
	GLfloat sampleBias;

	float lerp(float a, float b, float c);
	void generateUniformData(int width, int height);
};
