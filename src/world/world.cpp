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
#include "renderpass/lightingrenderpass.hpp"
#include "renderpass/particlerenderpass.hpp"

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
	// Gotta add particles render pass.
	{
		std::unique_ptr<GeometryRenderPass> geometry = std::make_unique<GeometryRenderPass>();
		std::unique_ptr<LightingRenderPass> lighting = std::make_unique<LightingRenderPass>();
		std::unique_ptr<ParticleRenderPass> particles = std::make_unique<ParticleRenderPass>();

		lighting->attachInputTexture(LightingRenderPass::InputAttachment::position, geometry->getAttachment(GeometryRenderPass::Attachment::position))
			.attachInputTexture(LightingRenderPass::InputAttachment::normal, geometry->getAttachment(GeometryRenderPass::Attachment::normal))
			.attachInputTexture(LightingRenderPass::InputAttachment::diffuseSpecular, geometry->getAttachment(GeometryRenderPass::Attachment::diffuseSpecular));

		_systems.push_back(std::move(geometry));
		_systems.push_back(std::move(lighting));
		_systems.push_back(std::move(particles));
	}
}
