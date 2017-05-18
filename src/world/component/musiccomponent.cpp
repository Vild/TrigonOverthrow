#include "musiccomponent.hpp"

#include "../../engine.hpp"

MusicComponent::MusicComponent(std::unique_ptr<Music> music) : music(std::move(music)) {}

MusicComponent::MusicComponent(const ComponentValues& value) {
	music = Engine::getInstance().getAudioManager()->getMusic(value.getString("file", ""));
	music->volume = value.getInt("volume", 1);
	if (value.getBool("autoplay", true))
		music->play(value.getBool("loops", -1));
}

MusicComponent::~MusicComponent() {}

void MusicComponent::registerImGui() {}