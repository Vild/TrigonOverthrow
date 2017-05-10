#include "infiniteplanefollowsystem.hpp"
#include "../component/infiniteplanefollowcomponent.hpp"

void InfinitePlaneFollowSystem::update(World & world, float delta)
{
	for (Entity * entity : Entity::getEntities<InfinitePlaneFollowComponent>())
	{
		auto ipfc = entity->getComponent<InfinitePlaneFollowComponent>();
		auto transform = entity->getComponent<TransformComponent>();

		auto pos = ipfc->followTransform->getPosition();
		pos.y = ipfc->followHeight;
		transform->setPosition(pos);
	}
}

void InfinitePlaneFollowSystem::registerImGui()
{
}

std::string InfinitePlaneFollowSystem::name()
{
	return "InfinitePlaneFollowSystem";
}
