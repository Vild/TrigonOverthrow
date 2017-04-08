#pragma once

#include "renderpass.hpp"

class BaseRenderPass : public RenderPass {
public:
	BaseRenderPass();

	void render(World& world);
};
