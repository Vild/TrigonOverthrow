// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "particlecomponent.hpp"

ParticleComponent::ParticleComponent(const ComponentValues& value) : ParticleComponent() {
	nrOfParticles = value.getInt("nrOfParticles", 256);
	glm::vec3 pos = value.getVec3("position", {0, 0, 0});
	glm::vec3 direction = value.getVec3("direction", { 0, 1, 0 });
	type = (ParticleEffect)value.getInt("particleEffect", 4);
	addEmitter(pos, direction, type);
}

ParticleComponent::~ParticleComponent() {}
void ParticleComponent::registerImGui() {}
