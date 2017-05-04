#pragma once
#include "system.hpp"
#include "../../gl/mesh.hpp"

class DynamicModelSystem : public System {
public:
	DynamicModelSystem();
	virtual ~DynamicModelSystem() {}
	void update(World& world, float delta) final;
	void registerImGui() final;
	inline std::string name() final { return "DynamicModelSystem"; }

private:
};
