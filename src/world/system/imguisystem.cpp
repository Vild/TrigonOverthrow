#include "imguisystem.hpp"

#include "../../lib/imgui.h"

void ImGuiSystem::update(World& world, float delta) {
	ImGui::Text("Entities:");
	for (std::shared_ptr<Entity> entity : world.getEntities()) {
		if (ImGui::TreeNode(entity->getName().c_str())) {
			for (std::shared_ptr<IComponent> component : entity->getComponents()) {
				if (ImGui::TreeNode(component->name().c_str())) {
					component->registerImGui();
					ImGui::TreePop();
				}
			}

			ImGui::TreePop();
		}
	}
}
