#include "experiencecomponent.hpp"

ExperienceComponent::ExperienceComponent(const ComponentValues& value) : ExperienceComponent() {
	currExp = value.getInt("currExp", 0);
	expToNextLevel = value.getInt("expToNextLevel", 5);
}

ExperienceComponent::~ExperienceComponent() {

}

void ExperienceComponent::registerImGui() {

}