#pragma once

#include "system.hpp"
#include "../../gl/gbuffer.hpp"
#include "../../gl/shader.hpp"
#include "../component/particlecomponent.hpp"

class ParticleSystem : public System {
public:
	enum Attachment : GLint { inPosition = 0, inVelocity, outPosition, outVelocity };

	ParticleSystem();
	virtual ~ParticleSystem();

	void update(World& world, float delta) final;
	void registerImGui() final;
	int getTextureSize() const { return _textureSize; }
	int getCurrEmitterCount() const { return _currEmitterCount; }
	inline std::string name() final { return "ParticleSystem"; };
	std::shared_ptr<GBuffer> getGBuffers();

private:
	void _addEmitterUniforms();
	void _setEmitterUniforms(std::vector<ParticleComponent::Emitter> emitters, const int emitterCount);
	std::vector<std::shared_ptr<ShaderProgram>> _programs;
	std::shared_ptr<GBuffer> _particleData;
	int _textureSize;
	int _currEmitterCount;
};
