#include "geometryrenderpass.hpp"

#include "../../engine.hpp"

GeometryRenderPass::GeometryRenderPass() {
	auto& engine = Engine::getInstance();

	_gbuffer = std::make_shared<GBuffer>();

	_gbuffer
		->attachTexture(Attachment::position, engine.getWidth(), engine.getHeight(), GL_RGB32F, GL_FLOAT, 3)
		.attachTexture(Attachment::normal, engine.getWidth(), engine.getHeight(), GL_RGB, GL_UNSIGNED_BYTE, 3)
		.attachTexture(Attachment::diffuseSpecular, engine.getWidth(), engine.getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, 4)
		.attachDepthTexture(Attachment::depth, engine.getWidth(), engine.getHeight())
		.attachRenderBuffer(engine.getWidth(), engine.getHeight(), GL_STENCIL_COMPONENT)
		.finalize();

	_shader = std::make_shared<ShaderProgram>();
	_shader->attach(std::make_shared<ShaderUnit>("assets/shaders/base.vert", ShaderType::vertex))
		.attach(std::make_shared<ShaderUnit>("assets/shaders/base.geom", ShaderType::geometry))
		.attach(std::make_shared<ShaderUnit>("assets/shaders/base.frag", ShaderType::fragment))
		.finalize();
	_shader->bind()
		.addUniform("v")
		.addUniform("p")
		.addUniform("s")
		.addUniform("cameraPos")
		.addUniform("diffuseTexture")
		.addUniform("normalTexture")
		.addUniform("setting_doBackFaceCulling")
		.addUniform("setting_defaultSpecular");
	_shader->setUniform("diffuseTexture", 0)
		.setUniform("normalTexture", 1)
		.setUniform("setting_doBackFaceCulling", _setting_base_doBackFaceCulling)
		.setUniform("setting_defaultSpecular", _setting_base_defaultSpecular);
}

void GeometryRenderPass::render(World& world) {
	_shader.bind();

	auto camera = Engine::getInstance().getCamera();
	if (!camera)
		return;

	auto cameraTransform = camera->getComponent<TransformComponent>();
	if (!cameraTransform)
		return;

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_shader->bind();

	float _fov = 80.0f; // XXX: Extract to CameraEntity

	// Everything needed for the view matrix is in the rotation matrix
	_shader->setUniform("v", cameraTransform->rotation);
	_shader->setUniform("p", glm::perspective(glm::radians(_fov), (float)1280 / (float)720, 0.1f, 60.0f));

	for (std::shared_ptr<Entity> entity : world.getEntities()) {
		auto model = entity->getComponent<ModelComponent>();
		if (!model)
			continue;

		auto transform = entity->getComponent<TransformComponent>();
		if (!transform)
			continue;

		_shader->setUniform("m", glm::translate(transform->position) * glm::scale(transform->scale) * transform->rotation);

		model->render();
	}

}