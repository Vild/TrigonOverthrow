#pragma once

#include "renderpass.hpp"

class FinalRenderPass : public RenderPass {
public:
	FinalRenderPass();

	void render(World& world);
};
