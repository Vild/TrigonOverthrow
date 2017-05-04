#pragma once

#include "component.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

struct HitboxComponent : public Component {
	enum HitboxType : int { SPHERE = 0, TETRAHEDRON, AABB };

	struct Hitbox {};

	struct HitboxSphere : public Hitbox {
		glm::vec3 center;
		float radius2;
	};

	struct HitboxAABB : public Hitbox {};

	HitboxType type;
	std::shared_ptr<Hitbox> hitbox;

	virtual ~HitboxComponent();

	void addHitbox(HitboxType type, glm::vec3 pos);
	void registerImGui() final;
	inline std::string name() final { return "HitboxComponent"; }
};
