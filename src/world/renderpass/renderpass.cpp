#include "renderpass.hpp"

void RenderPass::attachInputTexture(GLuint id, std::shared_ptr<Texture> texture) {
	_inputs[id] = texture;
}

void RenderPass::update(World& world, float delta) {
	if (!_gbuffer || !_shader)
		return;

	_gbuffer->bind();
	for (auto it = _inputs.begin(); it != _inputs.end(); ++it)
		it->second->bind(it->first);
	render(world);
}
