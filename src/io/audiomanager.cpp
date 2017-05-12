#include "audiomanager.hpp"
#include <glm/gtx/vector_angle.hpp>

#include <cstdio>

SFX::SFX(Mix_Chunk* chunk, int channel) : chunk(chunk), channel(channel), volume(MIX_MAX_VOLUME), position(glm::vec3{0, 0, 0}) {}

SFX::~SFX() {
	stop();
}

void SFX::play(int loops, glm::vec3 playerPosition, glm::vec3 forward) {
	update(playerPosition, forward);
	Mix_PlayChannel(channel, chunk, loops);
}

void SFX::stop() {
	Mix_HaltChannel(channel);
}

void SFX::update(glm::vec3 playerPosition, glm::vec3 forward) {
	Sint16 angel = glm::angle(glm::normalize(position - playerPosition), forward);
	Mix_SetPosition(channel, angel, glm::distance(playerPosition, position));
	Mix_Volume(channel, volume);
}

Music* Music::currentMusic = currentMusic;

Music::Music(Mix_Music* music) : music(music), volume(MIX_MAX_VOLUME) {}
Music::~Music() {
	stop();

	if (currentMusic == this)
		currentMusic = nullptr;
}

void Music::play(int loops) {
	currentMusic = this;
	update();
	Mix_PlayMusic(music, loops);
}

void Music::stop() {
	if (currentMusic != this)
		return;
	Mix_HaltMusic();
}

void Music::update() {
	Mix_VolumeMusic(volume);
}

AudioManager::AudioManager() {}

AudioManager::~AudioManager() {
	clear();
}

void AudioManager::clear() {
	for (std::pair<std::string, Mix_Chunk*> p : _sfxCache)
		Mix_FreeChunk(p.second);
	_sfxCache.clear();

	for (std::pair<std::string, Mix_Music*> p : _musicCache)
		Mix_FreeMusic(p.second);
	_musicCache.clear();
}

std::unique_ptr<SFX> AudioManager::getSFX(const std::string& file) {
	auto& chunk = _sfxCache[file];
	if (!chunk) {
		printf("Loading SFX: %s\n", file.c_str());
		chunk = Mix_LoadWAV(file.c_str());
		if (!chunk) {
			fprintf(stderr, "\tMix_LoadWAV: %s\n", Mix_GetError());
			return std::unique_ptr<SFX>();
		}
	}

	Mix_AllocateChannels(_channelCount + 1);
	return std::make_unique<SFX>(chunk, _channelCount++);
}

std::unique_ptr<Music> AudioManager::getMusic(const std::string& file) {
	auto& music = _musicCache[file];
	if (!music) {
		printf("Loading Music: %s\n", file.c_str());
		music = Mix_LoadMUS(file.c_str());
		if (!music) {
			fprintf(stderr, "\tMix_LoadMUS: %s\n", Mix_GetError());
			return std::unique_ptr<Music>();
		}
	}

	return std::make_unique<Music>(music);
}