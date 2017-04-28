#include "luasystem.hpp"
#include "../component/transformcomponent.hpp"
#include "../component/luacomponent.hpp"
void LuaSystem::update(World& world, float delta) {
	for (std::unique_ptr<Entity>& entity : world.getEntities()) {
		auto lua = entity->getComponent<LuaComponent>();
		if (!lua)
			continue;
		auto transform = entity->getComponent<TransformComponent>();
		if (transform)
		{
			if (entity->getName() == "Player")
			{
				_player = entity.get();
				//can't update player pos this way because lua doesnt understand vec3 - only understands doubles, not floats.
				//lua->L["updatePlayerPos"]((double)transform->position.x, (double)transform->position.y, (double)transform->position.z);
			}
			else {
				//vec3 issue, doesnt understand floats, only doubles.
				//transform->registerLua(lua->L);
				if (_player != nullptr)
				{
					//sending X and Z value of enemy and player and updating enemy position based on that, moving it closer to player position.
					posX = lua->L["update"]((double)delta,(double) transform->getPosition().x,(double) _player->getComponent<TransformComponent>()->getPosition().x);
					posZ = lua->L["update"]((double)delta, (double)transform->getPosition().z, (double)_player->getComponent<TransformComponent>()->getPosition().z);

					transform->setPosition(glm::vec3(posX, transform->getPosition().y, posZ));

				}
			}
		}
	}
}

void LuaSystem::registerImGui() {}
