#pragma once

#include "system.hpp"

class AudioSystem : public System {
public:
	virtual ~AudioSystem();

	void update(World& world, float delta) final;

	void registerImGui() final;
	inline std::string name() final { return "AudioSystem"; };
};
