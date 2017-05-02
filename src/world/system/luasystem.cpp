#include "luasystem.hpp"
#include "../component/transformcomponent.hpp"
#include "../component/rigidbodycomponent.hpp"
#include "../component/luacomponent.hpp"
void LuaSystem::update(World& world, float delta) {
	for (std::unique_ptr<Entity>& entity : world.getEntities()) {
		auto lua = entity->getComponent<LuaComponent>();
		if (!lua)
			continue;
		auto rigidBodyComponent = entity->getComponent<RigidBodyComponent>();
		auto transform = entity->getComponent<TransformComponent>();
		if (rigidBodyComponent)
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
					auto rb = rigidBodyComponent->getRigidBody();
					//sending X and Z value of enemy and player and updating enemy position based on that, moving it closer to player position.
					glm::vec3 playerPosition = _player->getComponent<TransformComponent>()->getPosition();
					glm::vec3 enemyPosition = transform->getPosition();
					int posX = lua->L["update"]((double)enemyPosition.x, (double)playerPosition.x);
					int posZ = lua->L["update"]((double)enemyPosition.z, (double)playerPosition.z);
					btVector3 force{(float)posX, 0,(float)posZ };
					rb->applyCentralForce(force*10);

				}
			}
		}
	}
}

void LuaSystem::registerImGui() {}
