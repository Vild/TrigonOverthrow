#pragma once
#include <map>
#include "system.hpp"
#include "../component/transformcomponent.hpp"

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

private:

	glm::ivec2 chunkSize = {32, 18};

	typedef std::pair<int, int> coord_t;
	typedef std::map<coord_t, Entity *> map_t;
	map_t chunks;


	void newRoom(World * world, coord_t coord);
	void loadBossRoom(World * world);

};