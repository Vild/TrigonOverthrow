#pragma once
#include "system.hpp"
#include "../component/transformcomponent.hpp"

class FloorTileSystem : public System
{
public:
	FloorTileSystem();
	virtual ~FloorTileSystem();

	void setPlayerTransform(TransformComponent * playerTransform);

	// Inherited via System
	virtual void update(World & world, float delta) override;

	virtual void registerImGui() override;

	virtual std::string name() override;
private:
	TransformComponent * playerTransform;

};