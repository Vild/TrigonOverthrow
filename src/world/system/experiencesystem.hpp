#pragma once
#include "system.hpp"

class ExperienceSystem : public System {
public:
	virtual ~ExperienceSystem();
	void update(World& world, float delta) final;
	void registerImGui() final;
	inline std::string name() final { return "GunSystem"; };
private:

};
