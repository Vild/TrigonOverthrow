#include "audiomanager.hpp"
#include <glm/gtx/vector_angle.hpp>

#include <cstdio>

SFX::SFX(std::shared_ptr<Mix_Chunk> chunk, int channel) : chunk(chunk), channel(channel), volume(MIX_MAX_VOLUME), position(glm::vec3{0, 0, 0}) {}

SFX::~SFX() {
	stop();
}

void SFX::play(int loops, glm::vec3 playerPosition, glm::vec3 forward) {
	Mix_PlayChannel(channel, chunk.get(), loops);
	update(playerPosition, forward);
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

Music::Music(std::shared_ptr<Mix_Music> music) : music(music), volume(MIX_MAX_VOLUME) {}
Music::~Music() {
	stop();

	if (currentMusic == this)
		currentMusic = nullptr;
}

void Music::play(int loops) {
	currentMusic = this;
	Mix_PlayMusic(music.get(), loops);
	update();
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

AudioManager::~AudioManager() {}

void AudioManager::clear() {
	_sfxCache.clear();
	_musicCache.clear();
}

std::unique_ptr<SFX> AudioManager::getSFX(const std::string& file) {
	auto& chunk = _sfxCache[file];
	if (!chunk) {
		printf("Loading SFX: %s\n", file.c_str());
		Mix_Chunk* tmpChunk = Mix_LoadWAV(file.c_str());
		if (!tmpChunk) {
			fprintf(stderr, "\tMix_LoadWAV: %s\n", Mix_GetError());
			return std::unique_ptr<SFX>();
		}
		chunk = std::shared_ptr<Mix_Chunk>(tmpChunk, &Mix_FreeChunk);
	}

	Mix_AllocateChannels(_channelCount + 1);
	return std::make_unique<SFX>(chunk, _channelCount++);
}

std::unique_ptr<Music> AudioManager::getMusic(const std::string& file) {
	auto& music = _musicCache[file];
	if (!music) {
		printf("Loading Music: %s\n", file.c_str());
		Mix_Music* tmpMusic = Mix_LoadMUS(file.c_str());
		if (!tmpMusic) {
			fprintf(stderr, "\tMix_LoadMUS: %s\n", Mix_GetError());
			return std::unique_ptr<Music>();
		}
		music = std::shared_ptr<Mix_Music>(tmpMusic, &Mix_FreeMusic);
	}

	return std::make_unique<Music>(music);
}