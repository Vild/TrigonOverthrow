#pragma once

#include "renderpass.hpp"

#include "../../gl/mesh.hpp"

class LightingRenderPass : public RenderPass {
public:
	enum InputAttachment : GLuint { position = 0, normal, diffuseSpecular };

	LightingRenderPass();

	void render(World& world);

private:
	std::shared_ptr<Mesh> _plane;
};
