#include "roomloadingsystem.hpp"
#include "../../engine.hpp"
#include "../../gl/simplemesh.hpp"
#include "../system/bulletphysicssystem.hpp"
#include "../component/instancedsimplemeshcomponent.hpp"
#include "../component/rigidbodycomponent.hpp"
#include "../component/transformcomponent.hpp"
#include "../component/floortilecomponent.hpp"
#include "../component/roomloadingcomponent.hpp"

#include "../../state/ingamestate.hpp"

RoomLoadingSystem::RoomLoadingSystem() {}

RoomLoadingSystem::~RoomLoadingSystem() {}

void RoomLoadingSystem::update(World& world, float delta) {
	if (!(typeid(Engine::getInstance().getState()) == typeid(InGameState)))
		return;

	auto player = Engine::getInstance().getState().getPlayer();
	auto playerTransform = player->getComponent<TransformComponent>();
	if (!playerTransform)
		return;

	glm::vec3 playerPos = playerTransform->getPosition() - glm::vec3(chunkSize.x * 0.5f, 0, chunkSize.y / 4.f);
	coord_t playerChunk = {playerPos.x / chunkSize.x, playerPos.z / chunkSize.y};

	for (size_t i = 0; i < 9; i++) {
		coord_t check = {playerChunk.first + (i % 3) - 1, playerChunk.second + (i / 3) - 1};
		map_t::iterator it = chunks.find(check);

		if (it == chunks.end()) {
			newRoom(&world, check);
		}
	}

	for (Entity* entity : world.getActiveComponents<RoomLoadingComponent>()) {
		RoomLoadingComponent* rlc = nullptr;
		if ((rlc = entity->getComponent<RoomLoadingComponent>())) {
			glm::ivec2 playerCoord = {playerChunk.first, playerChunk.second};
			glm::ivec2 roomCoord = rlc->getCoord();
			float distance = glm::distance(glm::vec2(playerCoord), glm::vec2(roomCoord));

			if (distance > 1.5) {
				rlc->unload();
				chunks.erase(coord_t(roomCoord.x, roomCoord.y));
			}
		}
	}
}

void RoomLoadingSystem::registerImGui() {}

std::string RoomLoadingSystem::name() {
	return "RoomLoadingSystem";
}

void RoomLoadingSystem::newRoom(World* world, coord_t coord) {
	static Engine* engine = &Engine::getInstance();
	static MapLoader* mapLoader = engine->getMapLoader().get();
	static JSONLoader* jsonLoader = engine->getJSONLoader().get();
	static BulletPhysicsSystem* bulletphyiscs = engine->getSystem<BulletPhysicsSystem>();

	static std::string maps[] = {"assets/maps/smileyface.json"};

	auto mapInfo = jsonLoader->loadMap(maps[0]);

	MapData map = mapLoader->loadFromImage("maps/test3218.png");
	Entity * room = world->addEntity(sole::uuid4(), "Room");
	
	auto rlc = room->addComponent<RoomLoadingComponent>(glm::ivec2(coord.first, coord.second));

	auto ismc = room->addComponent<InstancedSimpleMeshComponent>(std::make_unique<SimpleMesh>(GL_TRIANGLES, SimpleMesh::box));

	float offsetX = coord.first * chunkSize.x;
	float offsetY = coord.second * chunkSize.y;

	static auto randFloat = [] (float LO, float HI)
	{
		return LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
	};

	for (size_t i = 0, size = map.data.size(); i < size; i++)
	{
		int x = i % map.width;
		int y = i / map.width;
		float h = float(map.data[i]) / 128.f + randFloat(0, 0.1);

		Entity* tile = world->addEntity(sole::uuid4(), "FloorTile");
		tile->getHide() = true;

		TransformComponent* transform = tile->addComponent<TransformComponent>();

		transform->setScale({1, 3, 1});
		transform->setPosition({x + offsetX, 0.0f, y + offsetY});
		ismc->addInstance(transform);

		RigidBodyComponent* rigidbody = tile->addComponent<RigidBodyComponent>(tile);
		rigidbody->setTransform(transform);

		bulletphyiscs->addRigidBody(rigidbody, BulletPhysicsSystem::CollisionType::COL_WALL, BulletPhysicsSystem::wallCollidesWith);

		tile->addComponent<FloorTileComponent>(h);
		rlc->addEntity(tile);
	}

	rlc->addEntity(room);

	auto entities = mapInfo->constructEntities(*world);
	for (Entity* entity : entities) {
		rlc->addEntity(entity);
		TransformComponent* tr = entity->getComponent<TransformComponent>();
		RigidBodyComponent* rb = entity->getComponent<RigidBodyComponent>();
		if (tr) {
			tr->move({offsetX, 1.5f, offsetY});
			rb->setTransform(tr);
			bulletphyiscs->addRigidBody(rb, BulletPhysicsSystem::COL_ENEMY, BulletPhysicsSystem::enemyCollidesWith);
		}
	}

	chunks.insert_or_assign(coord, room);
}
