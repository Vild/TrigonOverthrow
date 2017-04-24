#include "luacomponent.hpp"

LuaComponent::~LuaComponent()
{
	lua_close(L);
}

void LuaComponent::loadFile(const char* fileName)
{
	int error = luaL_loadfile(L, fileName) || lua_pcall(L, 0, 0, 0);

	if (error)
	{
		printf("ERROR!");
		throw "Fail to load Lua";
	}
}