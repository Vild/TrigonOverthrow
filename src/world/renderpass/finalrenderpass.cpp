#include "finalrenderpass.hpp"

FinalRenderPass::FinalRenderPass() {
	_gbuffer = std::make_shared<GBuffer>(0);

	_shader = std::make_shared<ShaderProgram>();
	_shader->attach(std::make_shared<ShaderUnit>("assets/shaders/final.vert", ShaderType::vertex))
		.attach(std::make_shared<ShaderUnit>("assets/shaders/final.frag", ShaderType::fragment))
		.finalize();
	_shader->bind()
		.addUniform("vp")
		.addUniform("defPos")
		.addUniform("defNormal")
		.addUniform("defDiffuseSpecular");
}

void FinalRenderPass::render(World& world) {
}