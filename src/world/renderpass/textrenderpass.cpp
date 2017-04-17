#include "textrenderpass.hpp"

#include "../../engine.hpp"
#include "../component/cameracomponent.hpp"
#include "../component/textcomponent.hpp"
#include "../component/transformcomponent.hpp"

TextRenderPass::TextRenderPass() {
	_gbuffer = std::make_shared<GBuffer>(0);

	_shader = std::make_shared<ShaderProgram>();
	_shader->attach(std::make_shared<ShaderUnit>("assets/shaders/text.vert", ShaderType::vertex))
		.attach(std::make_shared<ShaderUnit>("assets/shaders/text.frag", ShaderType::fragment))
		.finalize();
	_shader->bind().addUniform("m").addUniform("vp").addUniform("fontMap");
	_shader->setUniform("fontMap", 0);
}

void TextRenderPass::render(World& world) {
	auto camera = Engine::getInstance().getCamera();
	if (!camera)
		return;

	auto cameraComponent = camera->getComponent<CameraComponent>();
	if (!cameraComponent)
		return;

	_shader->bind();
	_shader->setUniform("vp", cameraComponent->projectionMatrix * cameraComponent->viewMatrix);
	Engine::getInstance().getTextFactory()->getFontMap()->bind(0);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	for (std::shared_ptr<Entity> entity : world.getEntities()) {
		auto text = entity->getComponent<TextComponent>();
		if (!text)
			continue;

		auto transform = entity->getComponent<TransformComponent>();
		if (!transform)
			continue;

		auto tr = text->textRenderer;

		_shader->setUniform("m", transform->matrix);

		tr->getMesh()->render(tr->getText().size());
	}

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}

void TextRenderPass::resize(unsigned int width, unsigned int height) {}

void TextRenderPass::registerImGui() {}
