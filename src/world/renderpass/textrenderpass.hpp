#pragma once

#include "renderpass.hpp"

class TextRenderPass : public RenderPass {
public:
	TextRenderPass();
	virtual void render(World& world);
	virtual void resize(unsigned int width, unsigned int height);
	virtual void registerImGui();
	inline virtual std::string name() { return "TextRenderPass"; };

private:
};
