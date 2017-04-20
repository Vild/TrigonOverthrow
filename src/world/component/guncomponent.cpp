#include "guncomponent.hpp"


void GunComponent::addGun(GunComponent::GunType inType, glm::vec3 inO, glm::vec3 inDir) {
	switch (inType)
	{
	case GunComponent::RAYGUN: {
		type = inType;
		std::shared_ptr<RayGun> raygun = std::make_shared<RayGun>();
		raygun->ray.o = inO;
		raygun->ray.dir = inDir;
		raygun->bounceCount = 0;
		gun = raygun;
		break;
	}
	case GunComponent::ENERGYGUN: {
		
		break;
	}
	default:
		break;
	}
}

void GunComponent::registerImGui() {
	
}