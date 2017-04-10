#pragma once
#include "renderpass.hpp"
class ParticleRenderPass : public RenderPass {
public:
	enum Attachment : GLint { position = 0, normal, diffuseSpecular, depth };

	ParticleRenderPass();

	virtual void render(World& world);
	virtual void resize(unsigned int width, unsigned int height);

private:
	bool _setting_base_doBackFaceCulling = false;
	float _setting_base_defaultSpecular = 0.5f;
};
