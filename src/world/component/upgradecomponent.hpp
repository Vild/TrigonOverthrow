#pragma once
#include "component.hpp"

struct UpgradeComponent : public Component {
	enum UpgradeType : int {};
	
	
	
	UpgradeComponent() = default;
	UpgradeComponent(const ComponentValues& value);
	virtual ~UpgradeComponent();
	inline std::string name() final { return "ParticleComponent"; }
	void registerImGui() final;
};