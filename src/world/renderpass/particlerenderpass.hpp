#pragma once

#include "renderpass.hpp"

#include "../../lib/glad.h"

class ParticleRenderPass : public RenderPass {
public:
	enum InputAttachment { position = 0, velocity };

	ParticleRenderPass();
	virtual ~ParticleRenderPass();

	virtual void render(World& world);
	virtual void resize(unsigned int width, unsigned int height);
	virtual void registerImGui();
	inline virtual std::string name() { return "ParticleRenderPass"; };

private:
};
