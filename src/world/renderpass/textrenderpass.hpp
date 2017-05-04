#pragma once

#include "renderpass.hpp"

class TextRenderPass : public RenderPass {
public:
	TextRenderPass();
	virtual ~TextRenderPass();

	void render(World& world) final;
	void resize(unsigned int width, unsigned int height) final;
	void registerImGui() final;
	inline std::string name() final { return "TextRenderPass"; };

private:
};
