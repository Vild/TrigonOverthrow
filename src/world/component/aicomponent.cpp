#include "aicomponent.hpp"

AIComponent::AIComponent(const ComponentValues& value) {
	moveType = (MovePattern)value.getInt("moveType", 0);
	usefulTimer = value.getFloat("usefulTimer", 0);
	doInverse = value.getBool("doInverse", false);
}

AIComponent::~AIComponent() {
	
}

void AIComponent::registerImGui() {
	
}