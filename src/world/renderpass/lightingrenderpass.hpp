#pragma once

#include "renderpass.hpp"

#include "../../gl/mesh.hpp"

class LightingRenderPass : public RenderPass {
public:
	enum InputAttachment : GLint { position = 0, normal, diffuseSpecular, depth};

	LightingRenderPass();

	virtual void render(World& world);
	virtual void resize(unsigned int width, unsigned int height);

private:
	std::shared_ptr<Mesh> _plane;
};
