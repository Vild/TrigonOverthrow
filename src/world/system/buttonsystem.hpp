#pragma once
#include "system.hpp"
#include "../entity.hpp"

class ButtonSystem : public System {
public:
	virtual ~ButtonSystem();

	void update(World& world, float delta) final;

	void registerImGui() final;
	inline std::string name() final { return "ButtonSystem"; };

private:
	constexpr static float _epsilon = 1e-8;
};
