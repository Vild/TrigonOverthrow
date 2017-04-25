#pragma once
#include "system.hpp"
#include "../entity.hpp"

class CameraSystem : public System {
public:
	virtual ~CameraSystem();

	virtual void update(World& world, float delta) override;

	virtual void registerImGui();
	inline virtual std::string name() { return "CameraSystem"; };
};
