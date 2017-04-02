#include "imgui.hpp"

#include "../../lib/imgui.h"

void ImGuiSystem::update(World& world, float delta) {
	ImGui::Text("Entities:");
	for (std::shared_ptr<Entity> entity : world.getEntities()) {
		ImGui::Separator();
		if (ImGui::CollapsingHeader(entity->getName().c_str())) {
			for (std::shared_ptr<IComponent> component : entity->getComponents()) {
				ImGui::Text(component->name().c_str());
				component->registerImGui();
				ImGui::Separator();
			}
		}
	}
}
