#pragma once

#include "renderpass.hpp"

#include "../../lib/glad.h"
#include "../../gl/mesh.hpp"

class ParticleRenderPass : public RenderPass {
public:
	ParticleRenderPass();
	virtual ~ParticleRenderPass();

	void render(World& world) final;
	void resize(unsigned int width, unsigned int height) final;
	void registerImGui() final;
	inline std::string name() final { return "ParticleRenderPass"; };

private:
	const int NR_OF_PARTICLES = 256;
};
