#pragma once

#include "renderpass.hpp"

#include "../../lib/glad.h"

class GeometryRenderPass : public RenderPass {
public:
	enum Attachment : GLint { position = 0, normal, diffuseSpecular, depth };

	GeometryRenderPass();

	virtual void render(World& world);
	virtual void resize(unsigned int width, unsigned int height);

	virtual void registerImGui();
	inline virtual std::string name() { return "GeometryRenderPass"; };

private:
	std::shared_ptr<ShaderProgram> _floorShader;

	bool _setting_base_doBackFaceCulling = false;
	float _setting_base_defaultSpecular = 0.5f;
};
