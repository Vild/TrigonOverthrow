#include "sfxcomponent.hpp"

#include "../../engine.hpp"
#include "transformcomponent.hpp"

SFXComponent::SFXComponent(std::unique_ptr<SFX> sfx) : sfx(std::move(sfx)) {}

SFXComponent::SFXComponent(const ComponentValues& value) {
	Engine& engine = Engine::getInstance();
	Entity* player = engine.getState().getPlayer();
	auto transform = player->getComponent<TransformComponent>();

	glm::vec3 forward = glm::vec3(glm::mat4_cast(transform->getRotation()) * glm::vec4(0, 0, -1, 0));

	sfx = Engine::getInstance().getAudioManager()->getSFX(value.getString("file", ""));
	if (value.getBool("autoplay", true))
		sfx->play(value.getBool("loops", -1), transform->getPosition(), forward);
}

SFXComponent::~SFXComponent() {}

void SFXComponent::registerImGui() {}