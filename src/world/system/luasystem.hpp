#pragma once
#include "system.hpp"
#include "../entity.hpp"

class LuaSystem : public System {
public:
	virtual void update(World& world, float delta) override;

	virtual void registerImGui();
	inline virtual std::string name() { return "LuaSystem"; };
	Entity * _player = nullptr;
};
