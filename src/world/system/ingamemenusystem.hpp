#pragma once
#include "system.hpp"

class InGameMenuSystem : public System {
public:
	virtual ~InGameMenuSystem();

	void update(World& world, float delta) final;

	typedef void (*updateUI_f)(State& state, bool create);
	updateUI_f updateUI;

	bool escapePressed;
	bool showUI = false;

	void registerImGui() final;
	inline std::string name() final { return "InGameMenuSystem"; }
};