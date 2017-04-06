#include "particlerendersystem.hpp"
#include "../component/particlecomponent.hpp"
#include "../component/transformcomponent.hpp"
#include "../src/engine.hpp"

ParticleRenderSystem::ParticleRenderSystem() {
	_program.bind().attach(std::make_shared<ShaderUnit>("assets/shaders/particles.vert", ShaderType::vertex))
		.attach(std::make_shared<ShaderUnit>("assets/shaders/particles.frag", ShaderType::fragment))
		.finalize();
	_program.addUniform("cameraRight_wPos")
		.addUniform("cameraUp_wPos")
		.addUniform("cameraPos")
		.addUniform("viewProj");
}

void ParticleRenderSystem::update(World& world, float delta) {
	// Render particles with instanced drawing.
	auto camera = Engine::getInstance().getCamera();
	if (!camera)
		return;

	camera = camera->getComponent<TransformComponent>();

	_program.bind();
	for (std::shared_ptr<Entity> entity : world.getEntities()) {
		auto particle = entity->getComponent<ParticleComponent>();
		if (!particle)
			continue;
		particle->_quad->uploadBufferArray("m", particle->matrices);
		particle->_quad->render(particle->_nrOfParticles);
	}

}