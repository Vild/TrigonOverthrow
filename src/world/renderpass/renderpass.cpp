#include "renderpass.hpp"

RenderPass& RenderPass::attachInputTexture(GLuint id, std::shared_ptr<Texture> texture) {
	_inputs[id] = texture;
	return *this;
}

void RenderPass::update(World& world, float delta) {
	_gbuffer->bind();
	for (auto it = _inputs.begin(); it != _inputs.end(); ++it)
		it->second->bind(it->first);
	render(world);
}
