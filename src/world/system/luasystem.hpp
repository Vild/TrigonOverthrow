#pragma once
#include "system.hpp"
#include "../entity.hpp"

class LuaSystem : public System {
public:
	virtual void update(World& world, float delta) override;

	virtual void registerImGui();
	inline virtual std::string name() { return "LuaSystem"; };
	double posX, posZ;
	Entity * _player = nullptr;
};
