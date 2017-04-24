#pragma once

#include "component.hpp"
#include <lua.hpp>
struct LuaComponent : public Component
{
	lua_State *L = luaL_newstate();
	std::string name() { return "LuaComponent"; };
	void loadFile(const char* fileName);
	virtual void registerImGui();
};