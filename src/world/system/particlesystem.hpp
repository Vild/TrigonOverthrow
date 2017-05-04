#pragma once

#include "system.hpp"
#include "../../gl/gbuffer.hpp"
#include "../../gl/shader.hpp"

class ParticleSystem : public System {
public:
	enum Attachment : GLint { inPosition = 0, inVelocity, outPosition, outVelocity };

	ParticleSystem();
	virtual ~ParticleSystem();

	void update(World& world, float delta) final;
	void registerImGui() final;
	inline std::string name() final { return "ParticleSystem"; };
	std::shared_ptr<GBuffer> getGBuffers();

private:
	std::vector<std::shared_ptr<ShaderProgram>> _programs;
	std::shared_ptr<GBuffer> _particleData;
	int _textureSize;
};
