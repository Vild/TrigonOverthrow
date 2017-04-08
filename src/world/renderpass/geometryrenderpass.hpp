#pragma once

#include "renderpass.hpp"

#include "../../lib/glad.h"

class GeometryRenderPass : public RenderPass {
public:
	enum Attachment : GLint {
		position = 0,
		normal,
		diffuseSpecular,
		depth
	};

	GeometryRenderPass();

	virtual void render(World& world);

private:
	bool _setting_base_doBackFaceCulling = false;
	float _setting_base_defaultSpecular = 0.5f;
};
