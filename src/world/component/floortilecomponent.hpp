#pragma once

#include "component.hpp"
#include "transformcomponent.hpp"

class FloorTileComponent : public Component {
public:
	FloorTileComponent() = default;
	FloorTileComponent(float height);
	FloorTileComponent(const ComponentValues& value);
	virtual ~FloorTileComponent();

	float getHeight();
	float getHeightFactor();

	void setHeightFactor(float heightFactor);

	// Inherited via Component
	inline std::string name() final { return "FloorTileComponent"; }
	void registerImGui() final;

private:
	float height;
	float heightFactor;
};
