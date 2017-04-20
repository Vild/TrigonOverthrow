#pragma once
#include "component.hpp"
#include "glm/glm.hpp"


struct GunComponent : public Component<GunComponent> {
	enum GunType : int {RAYGUN = 0, ENERGYGUN};

	struct Gun {

	};

	struct RayGun : public Gun{
		struct Ray {
			glm::vec3 o;		// origin
			glm::vec3 dir;		// Direction
			glm::vec3 t[2];		// Intersection Data
		};
		Ray ray;
		int bounceCount;
	};

	struct EnergyGun : public Gun {

	};

	std::shared_ptr<Gun> gun;
	GunType type;

	void addGun(GunType inType, glm::vec3 inO, glm::vec3 inDir);
	void registerImGui();
	virtual std::string name() { return "GunComponent"; }
};