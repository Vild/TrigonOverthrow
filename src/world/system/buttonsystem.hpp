#pragma once
#include "system.hpp"
#include "../entity.hpp"

class ButtonSystem : public System {
public:
	virtual ~ButtonSystem();

	virtual void update(World& world, float delta);

	virtual void registerImGui();
	inline virtual std::string name() { return "ButtonSystem"; };

private:
	constexpr static float _epsilon = 1e-8;
};
