#pragma once

#include <memory>
#include <SDL2/SDL_mixer.h>
#include <glm/glm.hpp>
#include <map>

struct SFX {
	std::shared_ptr<Mix_Chunk> chunk;
	int channel;
	int volume;
	glm::vec3 position;

	SFX(std::shared_ptr<Mix_Chunk> chunk, int channel);
	virtual ~SFX();

	void play(int loops, glm::vec3 playerPosition, glm::vec3 forward = glm::vec3(0, 0, -1));
	void stop();

	void update(glm::vec3 playerPosition, glm::vec3 forward = glm::vec3(0, 0, -1));
};

struct Music {
	static Music* currentMusic; /// Used for determining if stop can be used

	std::shared_ptr<Mix_Music> music;
	int volume;

	Music(std::shared_ptr<Mix_Music> music);
	virtual ~Music();

	void play(int loops = -1);
	void stop();

	void update();
};

class AudioManager {
public:
	AudioManager();
	virtual ~AudioManager();

	void clear(); //XXX:

	std::unique_ptr<SFX> getSFX(const std::string& file);
	std::unique_ptr<Music> getMusic(const std::string& file);

private:
	std::map<std::string, std::shared_ptr<Mix_Chunk>> _sfxCache;
	std::map<std::string, std::shared_ptr<Mix_Music>> _musicCache;

	int _channelCount = 0;
};