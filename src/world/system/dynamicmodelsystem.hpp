#pragma once
#include "system.hpp"
#include "../../gl/mesh.hpp"

class DynamicModelSystem : public System {
public:
	DynamicModelSystem();
	virtual ~DynamicModelSystem() {}
	virtual void update(World& world, float delta);
	virtual void registerImGui();
	inline virtual std::string name() { return "DynamicModelSystem"; }
private:

};
