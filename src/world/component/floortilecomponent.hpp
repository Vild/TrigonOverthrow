#pragma once
#include "component.hpp"
#include "transformcomponent.hpp"

class FloorTileComponent : public Component
{
public:
	FloorTileComponent(float height);
	virtual ~FloorTileComponent();

	float getHeight();
	float getHeightFactor();

	void setHeightFactor(float heightFactor);

	// Inherited via Component 
	virtual std::string name() override;
	virtual void registerImGui() override;

private:
	float height;
	float heightFactor;
};