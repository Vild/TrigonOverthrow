// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "world.hpp"

#include "system/inputsystem.hpp"
#include "system/physicssystem.hpp"
#include "system/imguisystem.hpp"
#include "system/lookatsystem.hpp"
#include "system/camerasystem.hpp"
#include "system/particlesystem.hpp"

#include "entity/cameraentity.hpp"

#include "renderpass/geometryrenderpass.hpp"
#include "renderpass/ssaorenderpass.hpp"
#include "renderpass/lightingrenderpass.hpp"
#include "renderpass/particlerenderpass.hpp"
#include "renderpass/gaussianrenderpass.hpp"
#include "renderpass/textrenderpass.hpp"

#include <iostream>

World::World() {
	_setupSystems();
}

void World::tick(float delta) {
	for (const std::unique_ptr<System>& system : _systems)
		system->update(*this, delta);
}

void World::resize(unsigned int width, unsigned int height) {
	for (std::unique_ptr<System>& system : _systems) {
		RenderPass* rp = dynamic_cast<RenderPass*>(system.get());
		if (!rp)
			continue;
		rp->resize(width, height);
	}
}

void World::_setupSystems() {
	// Pure systems
	_systems.push_back(std::make_unique<ImGuiSystem>());
	_systems.push_back(std::make_unique<InputSystem>());
	_systems.push_back(std::make_unique<PhysicsSystem>());
	_systems.push_back(std::make_unique<LookAtSystem>());
	_systems.push_back(std::make_unique<CameraSystem>());
	_systems.push_back(std::make_unique<ParticleSystem>());

	// Render passes
	{
		std::unique_ptr<GeometryRenderPass> geometry = std::make_unique<GeometryRenderPass>();
		std::unique_ptr<SSAORenderSystem> ssao = std::make_unique<SSAORenderSystem>();
		std::unique_ptr<GaussianRenderPass> gaussian = std::make_unique<GaussianRenderPass>();
		std::unique_ptr<LightingRenderPass> lighting = std::make_unique<LightingRenderPass>();
		std::unique_ptr<ParticleRenderPass> particles = std::make_unique<ParticleRenderPass>(*this);
		std::unique_ptr<TextRenderPass> text = std::make_unique<TextRenderPass>();

		ssao->attachInputTexture(SSAORenderSystem::InputAttachments::PositionMap, geometry->getAttachment(GeometryRenderPass::Attachment::position));
		ssao->attachInputTexture(SSAORenderSystem::InputAttachments::NormalMap, geometry->getAttachment(GeometryRenderPass::Attachment::normal));

		gaussian->attachInputTexture(GaussianRenderPass::InputAttachments::Image, ssao->getAttachment(SSAORenderSystem::Attachments::OcclusionMap));

		lighting->attachInputTexture(LightingRenderPass::InputAttachment::position, geometry->getAttachment(GeometryRenderPass::Attachment::position))
			.attachInputTexture(LightingRenderPass::InputAttachment::normal, geometry->getAttachment(GeometryRenderPass::Attachment::normal))
			.attachInputTexture(LightingRenderPass::InputAttachment::diffuseSpecular, geometry->getAttachment(GeometryRenderPass::Attachment::diffuseSpecular))
			.attachInputTexture(LightingRenderPass::InputAttachment::depth, geometry->getAttachment(GeometryRenderPass::Attachment::depth))
			.attachInputTexture(LightingRenderPass::InputAttachment::OcclusionMap, gaussian->getAttachment(GaussianRenderPass::Attachments::BlurredImage));

		for (std::unique_ptr<System>& system : _systems) {
			auto particleSystem = dynamic_cast<ParticleSystem*>(system.get());
			if (!particleSystem)
				continue;
			auto _gbuffer = particleSystem->getGBuffers();

			// GIT-GUD: fixed in velocity and position to be output instead.
			particles->attachInputTexture(ParticleRenderPass::InputAttachment::position, _gbuffer->getAttachments()[ParticleSystem::Attachment::inPosition])
				.attachInputTexture(ParticleRenderPass::InputAttachment::velocity, _gbuffer->getAttachments()[ParticleSystem::Attachment::inVelocity]);

			break;
		}

		_systems.push_back(std::move(geometry));
		_systems.push_back(std::move(ssao));
		_systems.push_back(std::move(gaussian));
		_systems.push_back(std::move(lighting));
		_systems.push_back(std::move(particles));
		_systems.push_back(std::move(text));
	}
}
