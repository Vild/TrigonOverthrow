#include "particlerendersystem.hpp"
#include "../component/particlecomponent.hpp"
#include "../component/transformcomponent.hpp"
#include "../src/engine.hpp"
#include <glm/gtx/transform.hpp>
#include <cmath>

ParticleRenderSystem::ParticleRenderSystem() {
	_program.bind().attach(std::make_shared<ShaderUnit>("assets/shaders/particles.vert", ShaderType::vertex))
		.attach(std::make_shared<ShaderUnit>("assets/shaders/particles.frag", ShaderType::fragment))
		.finalize();
	_program.bind().addUniform("cameraRight_wPos")
		.addUniform("cameraUp_wPos")
		.addUniform("billboardSize")
		.addUniform("cameraPos")
		.addUniform("v")
		.addUniform("p");
}

void ParticleRenderSystem::update(World& world, float delta) {
	// Render particles with instanced drawing.
	auto camera = Engine::getInstance().getCamera();
	if (!camera)
		return;

	auto cameraTransform = camera->getComponent<TransformComponent>();
	if (!cameraTransform)
		return;

	float _fov = 80.0f; // XXX: Extract to CameraEntity

	_program.bind().setUniform("cameraRight_wPos", glm::vec3(-1, 0, 0))
		.setUniform("cameraUp_wPos", glm::vec3(0, 0.894, 0.447))
		.setUniform("cameraPos", cameraTransform->position)
		.setUniform("v", cameraTransform->rotation)
		.setUniform("p", glm::perspective(glm::radians(_fov), (float)1280 / (float)720, 0.1f, 60.0f));

	for (std::shared_ptr<Entity> entity : world.getEntities()) {
		auto particle = entity->getComponent<ParticleComponent>();
		if (!particle)
			continue;
		_program.setUniform("billboardSize", particle->particleSize);
		particle->_quad->uploadBufferArray("m", particle->matrices);
		particle->_quad->render(particle->_nrOfParticles);
	}

}