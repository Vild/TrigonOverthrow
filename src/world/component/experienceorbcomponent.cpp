#include "experienceorbcomponent.hpp"

ExperienceOrbComponent::ExperienceOrbComponent(const ComponentValues& value) : ExperienceOrbComponent() {
	amntOfExp = value.getInt("amntOfExp", 5);
}

ExperienceOrbComponent::~ExperienceOrbComponent() {

}

void ExperienceOrbComponent::registerImGui() {

}