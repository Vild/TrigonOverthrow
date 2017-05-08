// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "textrenderpass.hpp"

#include "../../engine.hpp"
#include "../component/cameracomponent.hpp"
#include "../component/textcomponent.hpp"
#include "../component/transformcomponent.hpp"

#include <glm/gtx/transform.hpp>

TextRenderPass::TextRenderPass() {
	_gbuffer = std::make_shared<GBuffer>(0);

	_shader = std::make_shared<ShaderProgram>();
	_shader->attach(std::make_shared<ShaderUnit>("assets/shaders/text.vert", ShaderType::vertex))
		.attach(std::make_shared<ShaderUnit>("assets/shaders/text.geom", ShaderType::geometry))
		.attach(std::make_shared<ShaderUnit>("assets/shaders/text.frag", ShaderType::fragment))
		.finalize();
	_shader->bind().addUniform("m").addUniform("vp").addUniform("fontMap");
	_shader->setUniform("fontMap", 0);
}

TextRenderPass::~TextRenderPass() {}

void TextRenderPass::render(World& world) {
	rmt_ScopedCPUSample(TextRenderPass, RMTSF_None);
	rmt_ScopedOpenGLSample(TextRenderPass);
	auto camera = Engine::getInstance().getCamera();
	if (!camera)
		return;

	auto cameraComponent = camera->getComponent<CameraComponent>();
	if (!cameraComponent)
		return;

	_shader->bind();
	_shader->setUniform("vp", cameraComponent->projectionMatrix * cameraComponent->viewMatrix);
	Engine::getInstance().getTextFactory()->getFontMap()->bind(0);

	glDisable(GL_CULL_FACE);

	for (std::unique_ptr<Entity>& entity : world.getEntities()) {
		auto text = entity->getComponent<TextComponent>();
		if (!text)
			continue;

		auto transform = entity->getComponent<TransformComponent>();
		if (!transform)
			continue;

		auto tr = text->textRenderer;

		// TODO: Remove Y(?)-axis rotation from transform->getMatrix()
		// To make the text into a billboard
		glm::mat4 matrix = transform->getMatrix() * text->transform.getMatrix();
		_shader->setUniform("m", matrix);

		tr->getMesh()->render(tr->getText().size());
	}

	glEnable(GL_CULL_FACE);
}

void TextRenderPass::resize(unsigned int width, unsigned int height) {}

void TextRenderPass::registerImGui() {}
