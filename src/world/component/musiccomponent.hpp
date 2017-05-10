#pragma once

#include "component.hpp"
#include "../../io/audiomanager.hpp"

struct MusicComponent : public Component {
	std::unique_ptr<Music> music;

	MusicComponent() = default;
	MusicComponent(std::unique_ptr<Music> music);
	MusicComponent(const ComponentValues& value);
	virtual ~MusicComponent();

	void registerImGui() final;
	inline std::string name() final { return "MusicComponent"; }
};