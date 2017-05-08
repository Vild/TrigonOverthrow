#include "roomloadingsystem.hpp"
#include "../../engine.hpp"
#include "../../gl/simplemesh.hpp"
#include "../system/bulletphysicssystem.hpp"
#include "../component/instancedsimplemeshcomponent.hpp"
#include "../component/rigidbodycomponent.hpp"
#include "../component/transformcomponent.hpp"
#include "../component/floortilecomponent.hpp"

RoomLoadingSystem::RoomLoadingSystem()
{
}

RoomLoadingSystem::~RoomLoadingSystem()
{
}

void RoomLoadingSystem::setPlayerTransform(TransformComponent * playerTransform)
{
	this->playerTransform = playerTransform;
}

void RoomLoadingSystem::update(World & world, float delta)
{
	glm::vec3 playerPos = playerTransform->getPosition();
	coord_t playerChunk = { playerPos.x / chunkSize, playerPos.z / chunkSize };

	for (size_t i = 0; i < 9; i++)
	{
		coord_t check = {
			playerChunk.first + (i % 3) - 1,
			playerChunk.second + (i / 3) - 1
		};
		map_t::iterator it = chunks.find(check);

		if (it == chunks.end())
			newRoom(&world, check);
	}

}

void RoomLoadingSystem::registerImGui()
{
}

std::string RoomLoadingSystem::name()
{
	return "RoomLoadingSystem";
}

void RoomLoadingSystem::newRoom(World * world, coord_t coord)
{
	static Engine * engine = &Engine::getInstance();
	static MapLoader * mapLoader = engine->getMapLoader().get();
	static BulletPhysicsSystem * bulletphyiscs = engine->getSystem<BulletPhysicsSystem>();
	static std::string maps[] = { "maps/derp.png", "maps/sunkentemple.png" , "maps/whiteness.png", "maps/smileyface.png" };

	MapData map = mapLoader->loadFromImage(maps[(int)(rand() % 4)]);
	Entity * room = world->addEntity(sole::uuid4(), "Room");

	auto ismc = room->addComponent<InstancedSimpleMeshComponent>(
		std::make_unique<SimpleMesh>(GL_TRIANGLES, SimpleMesh::box));

	for (size_t i = 0, size = map.data.size(); i < size; i++)
	{
		int x = i % map.width;
		int y = i / map.width;
		float h = float(map.data[i]) / 255.f;

		Entity* tile = world->addEntity(sole::uuid4(), "FloorTile");
		tile->getHide() = true;

		TransformComponent * transform = tile->addComponent<TransformComponent>();
		float offsetX = coord.first * chunkSize;
		float offsetY = coord.second * chunkSize;
		transform->setScale({ 0.95, 1, 0.95 });
		transform->setPosition({ x + offsetX, 0.0f, y + offsetY });
		ismc->addInstance(transform);

		RigidBodyComponent * rigidbody = tile->addComponent<RigidBodyComponent>(tile);
		rigidbody->setTransform(transform);
		rigidbody->setHitboxHalfSize({ 0.5, 0.5, 0.5 });

		bulletphyiscs->addRigidBody(rigidbody,
			BulletPhysicsSystem::CollisionType::COL_WALL,
			BulletPhysicsSystem::wallCollidesWith);

		tile->addComponent<FloorTileComponent>(h);
	}

	chunks.insert_or_assign(coord, room);
}
