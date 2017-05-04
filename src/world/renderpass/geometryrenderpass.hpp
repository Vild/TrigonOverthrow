#pragma once

#include "renderpass.hpp"

#include "../../lib/glad.h"

class GeometryRenderPass : public RenderPass {
public:
	enum Attachment : GLint { position = 0, normal, diffuseSpecular, depth };

	GeometryRenderPass();
	virtual ~GeometryRenderPass();

	void render(World& world) final;
	void resize(unsigned int width, unsigned int height) final;

	void registerImGui() final;
	inline std::string name() final { return "GeometryRenderPass"; };

private:
	std::shared_ptr<ShaderProgram> _floorShader;
	std::unique_ptr<ShaderProgram> ismShader;

	bool _setting_base_doBackFaceCulling = false;
	float _setting_base_defaultSpecular = 0.5f;
};
