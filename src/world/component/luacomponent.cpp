#include "luacomponent.hpp"
#include "transformcomponent.hpp"

#include <glm/glm.hpp>

LuaComponent::LuaComponent()
{
	//doesnt understand floats, only doubles. Would otherwise use.
	//L["vec3"].SetClass<glm::vec3>("x", &glm::vec3::x, "y", &glm::vec3::y, "z", &glm::vec3::z);
}

LuaComponent::~LuaComponent()
{
}

void LuaComponent::loadFile(const char* fileName)
{
	L.Load(fileName);
}
void LuaComponent::registerImGui()
{

}