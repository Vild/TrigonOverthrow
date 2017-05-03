#pragma once
#include "system.hpp"

class HoverSystem : public System
{
public:
	HoverSystem();
	virtual ~HoverSystem();

	// Inherited via System
	virtual void update(World & world, float delta) override;
	virtual void registerImGui() override;
	virtual std::string name() override;
private:

};