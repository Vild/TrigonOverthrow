#include "luasystem.hpp"

#include "../component/luacomponent.hpp"
void LuaSystem::update(World& world, float delta) {
	for (std::unique_ptr<Entity>& entity : world.getEntities()) {
		auto lua = entity->getComponent<LuaComponent>();
		if (!lua)
			continue;
		lua_getglobal(lua->L, "update()");
		lua_pushnumber(lua->L, delta);
		int error = lua_pcall(lua->L, 1, 0, 0);
		if (error)
		{
			
			printf("Error message %s\n", lua_tostring(lua->L,-1));
			lua_pop(lua->L, 1);
		}

	}
}

void LuaSystem::registerImGui() {}
