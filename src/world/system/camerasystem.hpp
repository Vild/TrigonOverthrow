#pragma once
#include "system.hpp"
#include "../entity.hpp"

class CameraSystem : public System {
public:
	virtual ~CameraSystem();

	void update(World& world, float delta) final;

	void registerImGui() final;
	inline std::string name() final { return "CameraSystem"; };
};
