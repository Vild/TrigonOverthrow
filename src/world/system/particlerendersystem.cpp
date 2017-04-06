#include "particlerendersystem.hpp"
#include "../src/engine.hpp"

ParticleRenderSystem::ParticleRenderSystem() {
	_program.attach(std::make_shared<ShaderUnit>("assets/shaders/particles.vert", ShaderType::vertex))
		.attach(std::make_shared<ShaderUnit>("assets/shaders/particles.frag", ShaderType::fragment))
		.finalize();
}

void ParticleRenderSystem::update(World& world, float delta) {
	// Render particles with instanced drawing.
	auto camera = Engine::getInstance().getCamera();
	if (!camera)
		return;

}