#pragma once
#include "renderpass.hpp"
class ParticleRenderPass : public RenderPass {
public:
	ParticleRenderPass();

	virtual void render(World& world);
	virtual void resize(unsigned int width, unsigned int height);

private:

};
