#pragma once

#include "component.hpp"
#include "../../io/audiomanager.hpp"

struct SFXComponent : public Component {
	std::unique_ptr<SFX> sfx;

	SFXComponent() = default;
	SFXComponent(std::unique_ptr<SFX> sfx);
	SFXComponent(const ComponentValues& value);
	virtual ~SFXComponent();

	void registerImGui() final;
	inline std::string name() final { return "SFXComponent"; }
};