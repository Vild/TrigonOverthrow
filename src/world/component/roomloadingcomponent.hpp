#pragma once
#include "component.hpp"
#include <vector>

class Entity;

class RoomLoadingComponent : public Component
{
public:
	RoomLoadingComponent(glm::ivec2 coord);
	virtual ~RoomLoadingComponent();

	void unload();

	void addEntity(Entity * entity);

	// Inherited via Component
	virtual std::string name() override;
	virtual void registerImGui() override;

	inline glm::ivec2& getCoord() { return coord; }
private:
	glm::ivec2 coord;
	std::vector<Entity*> entities;

};

