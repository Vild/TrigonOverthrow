#include "world.hpp"

#include "system/inputsystem.hpp"
#include "system/physicssystem.hpp"
#include "system/imguisystem.hpp"
#include "system/lookatsystem.hpp"

#include "entity/cameraentity.hpp"

#include "renderpass/geometryrenderpass.hpp"
#include "renderpass/lightingrenderpass.hpp"

#include <iostream>

World::World() {
	_setupSystems();
}

void World::tick(float delta) {
	std::cout << "====== TICK START ======" << std::endl;
	for (const std::unique_ptr<System>& system : _systems) {
		std::cout << (typeid(*system).name() + 2) << std::endl;
		system->update(*this, delta);
	}

	std::cout << "====== TICK END   ======" << std::endl << std::endl << std::endl;
}

void World::_setupSystems() {
	// Pure systems
	_systems.push_back(std::make_unique<ImGuiSystem>());
	_systems.push_back(std::make_unique<InputSystem>());
	_systems.push_back(std::make_unique<PhysicsSystem>());
	_systems.push_back(std::make_unique<LookAtSystem>());

	// Render passes
	{
		std::unique_ptr<GeometryRenderPass> geometry = std::make_unique<GeometryRenderPass>();
		std::unique_ptr<LightingRenderPass> lighting = std::make_unique<LightingRenderPass>();
		lighting->attachInputTexture(LightingRenderPass::InputAttachment::position, geometry->getAttachment(GeometryRenderPass::Attachment::position))
			.attachInputTexture(LightingRenderPass::InputAttachment::normal, geometry->getAttachment(GeometryRenderPass::Attachment::normal))
			.attachInputTexture(LightingRenderPass::InputAttachment::diffuseSpecular, geometry->getAttachment(GeometryRenderPass::Attachment::diffuseSpecular));

		_systems.push_back(std::move(geometry));
		_systems.push_back(std::move(lighting));
	}
}
