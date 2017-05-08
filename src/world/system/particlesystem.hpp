#pragma once

#include "system.hpp"
#include "../../gl/gbuffer.hpp"
#include "../../gl/shader.hpp"
#include "../component/particlecomponent.hpp"

class ParticleSystem : public System {
public:
	enum Attachment : GLint { inPosition = 0, inVelocity, outPosition, outVelocity };
	enum ParticleAttribute : int { position = 0, velocity, life};

	ParticleSystem();
	virtual ~ParticleSystem();

	void update(World& world, float delta) final;
	void registerImGui() final;
	std::vector<std::shared_ptr<ShaderStorageBuffer>> getSSBO() { return _ssbos; }
	inline std::string name() final { return "ParticleSystem"; }
	std::shared_ptr<GBuffer> getGBuffers();
	int getEmitterCount() const { return nrOfEmitters; }

private:
	void _removeEmitter(const int counter);
	void _addNewData(int emitters[64], int tempNrOfEmitters);
	std::vector<std::shared_ptr<ShaderProgram>> _programs;
	std::vector<std::shared_ptr<ShaderStorageBuffer>> _ssbos;
	std::vector<glm::vec4> _computePositions;
	std::vector<glm::vec4> _computeVelocities;
	std::vector<float> _computeLives;
	const int MAX_EMITTER_COUNT = 64;
	const int NR_OF_PARTICLES = 1024;
	unsigned int nrOfEmitters;
};
