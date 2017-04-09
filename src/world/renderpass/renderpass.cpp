// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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
