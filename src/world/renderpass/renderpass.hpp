#pragma once

#include "../system/system.hpp"
#include "../../gl/gbuffer.hpp"
#include "../../gl/shader.hpp"

#include <map>

class RenderPass : public System {
public:
	virtual ~RenderPass() = 0;

	RenderPass& attachInputTexture(GLuint id, std::shared_ptr<Texture> texture);

	virtual void render(World& world) = 0;
	void update(World& world, float delta) final;

	virtual void resize(unsigned int width, unsigned int height) = 0;

	inline std::shared_ptr<GBuffer> getGBuffer() { return _gbuffer; }
	inline std::shared_ptr<ShaderProgram> getShader() { return _shader; }

	inline std::shared_ptr<Texture> getAttachment(int id) { return _gbuffer->getAttachments()[id]; }

protected:
	std::shared_ptr<GBuffer> _gbuffer;
	std::shared_ptr<ShaderProgram> _shader;
	std::map<GLuint, std::shared_ptr<Texture>> _inputs;
};

inline RenderPass::~RenderPass() {}
