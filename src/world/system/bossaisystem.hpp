#pragma once
#include "system.hpp"
#include "../component/aicomponent.hpp"

class BossAISystem : public System {
public:
	BossAISystem();
	virtual ~BossAISystem();
	void update(World& world, float delta) final;
	void registerImGui() final;
	inline std::string name() final { return "BossAISystem"; }
	void setCentre(glm::vec3 centre) { _centre = centre; }
private:
	bool _walkToMiddle;
	glm::vec3 _centre;
	void _moveMiddle(Entity* entity);
};