#pragma once

#include "renderpass.hpp"

#include "../../lib/glad.h"

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
};
