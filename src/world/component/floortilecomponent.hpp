#pragma once

#include "component.hpp"
#include "transformcomponent.hpp"

class FloorTileComponent : public Component {
public:
	enum State {STATE_DYNAMIC, STATE_LOWERING, STATE_RISING, STATE_STATIC, STATE_FLUID};

	FloorTileComponent() = default;
	FloorTileComponent(float height);
	FloorTileComponent(const ComponentValues& value);
	virtual ~FloorTileComponent();

	State getState();
	float getHeight();
	float getProgress();
	float getHeightFactor();

	void setProgress(float progress);
	void setHeightFactor(float heightFactor);

	// Lowers the floor step by step and returns true if height = 0

	void riseFloor();
	void lowerFloor();
	void staticFloor();
	void dynamicFloor();

	bool isDone();


	// Inherited via Component
	inline std::string name() final { return "FloorTileComponent"; }
	void registerImGui() final;

private:
	float maxHeight;
	float height;
	float heightFactor;
	float progress;
	State state;
};
