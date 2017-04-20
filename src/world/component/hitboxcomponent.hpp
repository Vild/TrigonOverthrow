#pragma once

#include "component.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

struct HitboxComponent : public Component<HitboxComponent> {
	enum HitboxType : int {SPHERE = 0, TETRAHEDRON, AABB};

	struct Hitbox {

	};

	struct HitboxSphere : public Hitbox {
		glm::vec3 center;
		float radius2;
	};

	struct HitboxAABB : public Hitbox{

	};

	HitboxType type;
	std::shared_ptr<Hitbox> hitbox;

	void addHitbox(HitboxType type, glm::vec3 pos);
	virtual void registerImGui();
	virtual std::string name() { return "HitboxComponent"; }
};