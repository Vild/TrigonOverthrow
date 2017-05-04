#pragma once

#include "renderpass.hpp"

#include "../../lib/glad.h"
#include "../../gl/mesh.hpp"

class ParticleRenderPass : public RenderPass {
public:
	enum InputAttachment { position = 0, velocity };

	ParticleRenderPass();
	virtual ~ParticleRenderPass();

	void render(World& world) final;
	void resize(unsigned int width, unsigned int height) final;
	void registerImGui() final;
	inline std::string name() final { return "ParticleRenderPass"; };

private:
	std::shared_ptr<Mesh> _point;
	const int NR_OF_PARTICLES = 1024;
};
