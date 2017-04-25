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

		// XXX: text->transform.matrix * transform->matrix
		const glm::vec3 x(1, 0, 0);
		const glm::vec3 y(0, 1, 0);
		const glm::vec3 z(0, 0, 1);


		glm::vec3 position = transform->getPosition();
		glm::quat rotation = transform->getRotation();

		glm::mat4 matrix = transform->getMatrix() * text->transform.getMatrix();
		////matrix = glm::translate(transform->position);
		////matrix = matrix * glm::rotate(glm::radians(transform->rotation.z), z);
		////matrix = matrix * glm::rotate(glm::radians(transform->rotation.y), y);
		////matrix = matrix * glm::rotate(-glm::radians(transform->rotation.x), x);

		////matrix = matrix * glm::rotate(glm::radians(text->transform.rotation.z), z);
		////matrix = matrix * glm::rotate(glm::radians(text->transform.rotation.y), y);
		////matrix = matrix * glm::rotate(-glm::radians(text->transform.rotation.x), x);
		////matrix = matrix * glm::translate(text->transform.position);

		////matrix = matrix * glm::scale(text->transform.scale) * glm::scale(transform->scale);
		_shader->setUniform("m", matrix);

		tr->getMesh()->render(tr->getText().size());
	}

	glEnable(GL_CULL_FACE);
}

void TextRenderPass::resize(unsigned int width, unsigned int height) {}

void TextRenderPass::registerImGui() {}
