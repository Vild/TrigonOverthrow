#pragma once
#include <map>
#include "system.hpp"
#include "../component/transformcomponent.hpp"

const int ENEMIES_KILLED_BEFORE_BOSS = 16;

class RoomLoadingSystem : public System
{
public:
	enum class BossRoom { NO, LOADING, YES };
	RoomLoadingSystem();
	virtual ~RoomLoadingSystem();

	// Inherited via System
	virtual void update(World & world, float delta) override;
	virtual void registerImGui() override;
	virtual std::string name() override;

	BossRoom bossRoomLoaded;

	void spawnBossRoom(World & world);

	void enemyDead(World & world, Entity* entity);
	void reset() override
	{
		chunks.clear();
		bossRoomLoaded = BossRoom::NO;
		enemiesDead = 0;
		first = true;
	}

private:
	int enemiesDead;
	std::vector<std::shared_ptr<MapInformation>> _maps;
	bool first;
	glm::ivec2 chunkSize = {32, 18};

	typedef std::pair<int, int> coord_t;
	typedef std::map<coord_t, Entity *> map_t;
	map_t chunks;

	void spawnSpawnRoom(World & world);
	void newRoom(World * world, coord_t coord);
	void loadBossRoom(World * world);

};