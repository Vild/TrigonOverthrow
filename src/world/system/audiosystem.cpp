#include "audiosystem.hpp"

#include "../../engine.hpp"

#include "../component/transformcomponent.hpp"
#include "../component/sfxcomponent.hpp"
#include "../component/musiccomponent.hpp"

#include <glm/gtc/quaternion.hpp>

AudioSystem::~AudioSystem() {}

void AudioSystem::update(World& world, float delta) {
	Engine& engine = Engine::getInstance();
	Entity* player = engine.getState().getPlayer();
	if (!player) return;

	auto transform = player->getComponent<TransformComponent>();

	glm::vec4 forward = glm::vec4(0, 0, -1, 0);
	if (transform)
		forward = glm::mat4_cast(transform->getRotation()) * forward;

	for (Entity* entity : world.getActiveComponents<SFXComponent>()) {
		SFXComponent* sfx = entity->getComponent<SFXComponent>();
		sfx->sfx->update(transform->getPosition(), glm::vec3(forward));
	}

	for (Entity* entity : world.getActiveComponents<MusicComponent>()) {
		MusicComponent* music = entity->getComponent<MusicComponent>();
		music->music->update();
	}
}

void AudioSystem::registerImGui() {}