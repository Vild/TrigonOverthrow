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
	auto transform = player->getComponent<TransformComponent>();

	glm::vec3 forward = glm::vec3(glm::mat4_cast(transform->getRotation()) * glm::vec4(0, 0, -1, 0));

	for (Entity* entity : Entity::getEntities<SFXComponent>()) {
		SFXComponent* sfx = entity->getComponent<SFXComponent>();
		sfx->sfx->update(transform->getPosition(), forward);
	}

	for (Entity* entity : Entity::getEntities<MusicComponent>()) {
		MusicComponent* music = entity->getComponent<MusicComponent>();
		music->music->update();
	}
}

void AudioSystem::registerImGui() {}