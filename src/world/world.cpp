// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "world.hpp"

#include "system/inputsystem.hpp"
#include "system/physicssystem.hpp"
#include "system/imguisystem.hpp"
#include "system/lookatsystem.hpp"
#include "system/camerasystem.hpp"

#include "entity/cameraentity.hpp"

#include "renderpass/geometryrenderpass.hpp"
#include "renderpass/lightingrenderpass.hpp"
#include "renderpass/ssaorenderpass.hpp"
#include "renderpass/gaussianrenderpass.hpp"

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

	// Render passes
	{
		std::unique_ptr<GeometryRenderPass> geometry = std::make_unique<GeometryRenderPass>();
		std::unique_ptr<SSAORenderSystem> ssao = std::make_unique<SSAORenderSystem>();
		//std::unique_ptr<GaussianRenderPass> gaussian = std::make_unique<GaussianRenderPass>();
		std::unique_ptr<LightingRenderPass> lighting = std::make_unique<LightingRenderPass>();

		ssao->attachInputTexture(SSAORenderSystem::InputAttachments::PositionMap, geometry->getAttachment(GeometryRenderPass::Attachment::position));
		ssao->attachInputTexture(SSAORenderSystem::InputAttachments::NormalMap, geometry->getAttachment(GeometryRenderPass::Attachment::normal));

		//gaussian->attachInputTexture(GaussianRenderPass::InputAttachments::Image, ssao->getAttachment(SSAORenderSystem::Attachments::OcclusionMap));

		lighting->attachInputTexture(LightingRenderPass::InputAttachment::position, geometry->getAttachment(GeometryRenderPass::Attachment::position))
			.attachInputTexture(LightingRenderPass::InputAttachment::normal, geometry->getAttachment(GeometryRenderPass::Attachment::normal))
			.attachInputTexture(LightingRenderPass::InputAttachment::diffuseSpecular, geometry->getAttachment(GeometryRenderPass::Attachment::diffuseSpecular))
			.attachInputTexture(LightingRenderPass::InputAttachment::OcclusionMap, ssao->getAttachment(SSAORenderSystem::Attachments::OcclusionMap));

		_systems.push_back(std::move(geometry));
		_systems.push_back(std::move(ssao));
		//_systems.push_back(std::move(gaussian));
		_systems.push_back(std::move(lighting));
	}
}
