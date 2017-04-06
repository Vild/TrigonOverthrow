#pragma once

#include "renderpass.hpp"

class GeometryRenderPass : public RenderPass {
public:
	enum class Attachment : int {
		position,
		normal,
		diffuseSpecular,
		depth
	};

	GeometryRenderPass();

	virtual void render(World& world);

private:
	bool _setting_base_doBackFaceCulling = true;
	float _setting_base_defaultSpecular = 0.5f;
};