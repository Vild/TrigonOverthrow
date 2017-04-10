#include "particlerenderpass.hpp"
#include "../component/particlecomponent.hpp"
#include "../component/transformcomponent.hpp"
#include "../src/engine.hpp"
#include "../component/cameracomponent.hpp"
#include "../component/lookatcomponent.hpp"
#include <glm/gtx/transform.hpp>
#include <cmath>
#include "glm/glm.hpp"

ParticleRenderPass::ParticleRenderPass() {
	_gbuffer = std::make_shared<GBuffer>(0);
	_shader = std::make_shared<ShaderProgram>();
	_shader->bind().attach(std::make_shared<ShaderUnit>("assets/shaders/particles.vert", ShaderType::vertex))
		.attach(std::make_shared<ShaderUnit>("assets/shaders/particles.frag", ShaderType::fragment))
		.finalize();
	_shader->addUniform("cameraRight_wPos")
		.addUniform("cameraUp_wPos")
		.addUniform("billboardSize")
		.addUniform("cameraPos")
		.addUniform("v")
		.addUniform("p")
		.addUniform("particlePos");
}

void ParticleRenderPass::render(World& world) {
	// Render particles with instanced drawing.
	auto camera = Engine::getInstance().getCamera();
	if (!camera)
		return;

	auto cameraComponent = camera->getComponent<CameraComponent>();
	if (!cameraComponent)
		return;

	auto transformComponent = camera->getComponent<TransformComponent>();
	if (!transformComponent)
		return;

	glm::vec3 rightWPos = { cameraComponent->viewMatrix[0][0], cameraComponent->viewMatrix[0][1], cameraComponent->viewMatrix[0][2] };
	glm::vec3 upWpos = { cameraComponent->viewMatrix[0][1], cameraComponent->viewMatrix[1][1], cameraComponent->viewMatrix[2][1] };
	_shader->bind().setUniform("cameraRight_wPos", upWpos)
		.setUniform("cameraUp_wPos", rightWPos)
		.setUniform("cameraPos", transformComponent->position)
		.setUniform("v", cameraComponent->viewMatrix)
		.setUniform("p", cameraComponent->projectionMatrix);
	for (std::shared_ptr<Entity> entity : world.getEntities()) {
		auto particle = entity->getComponent<ParticleComponent>();
		if (!particle)
			continue;
		_shader->setUniform("billboardSize", particle->particleSize);

		glDrawArrays(GL_POINTS, 1, 1);
	}

}

void ParticleRenderPass::resize(unsigned int width, unsigned int height) {
	_gbuffer->bind();
	glViewport(0, 0, width, height);
}