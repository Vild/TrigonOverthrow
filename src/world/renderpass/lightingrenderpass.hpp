#pragma once

#include "renderpass.hpp"

#include "../../gl/mesh.hpp"

class LightingRenderPass : public RenderPass {
public:
	enum InputAttachment : GLint { position = 0, normal, diffuseSpecular, depth, OcclusionMap };

	LightingRenderPass();
	virtual ~LightingRenderPass();

	virtual void render(World& world);
	virtual void resize(unsigned int width, unsigned int height);

	virtual void registerImGui();
	inline virtual std::string name() { return "LightingRenderPass"; };

private:
	std::shared_ptr<Mesh> _plane;
};
