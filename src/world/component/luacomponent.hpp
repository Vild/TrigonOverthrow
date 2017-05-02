#pragma once

#include "component.hpp"
#include <lua.hpp>
#include <selene.h>
struct LuaComponent : public Component
{
	LuaComponent();
	virtual ~LuaComponent();
	sel::State L{ true };
	inline std::string name() { return "LuaComponent"; };
	void loadFile(const char* fileName);
	virtual void registerImGui();
};