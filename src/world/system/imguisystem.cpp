// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "imguisystem.hpp"

#include "../../lib/imgui.h"
#include "../../engine.hpp"

void ImGuiSystem::update(World& world, float delta) {
	State& state = Engine::getInstance().getState();

	ImGui::SetNextWindowPos(ImVec2(8, 48), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(384, 512), ImGuiSetCond_Once);
	ImGui::SetNextWindowCollapsed(true, ImGuiSetCond_Once);
	ImGui::Begin("Settings Window");

	ImGui::Text("Entities:");
	char name[255] = {0};
	for (std::unique_ptr<Entity>& entity : world.getEntities()) {
		snprintf(name, sizeof(name), "%s (%s)##%p", entity->getName().c_str(), entity->getUUID().str().c_str(), (void*)entity.get());
		if (ImGui::TreeNode(name)) {
			ImGui::Text("Actions:");
			if (entity->registerImGui)
				entity->registerImGui(*entity.get(), state);

			ImGui::Text("Component:");
			for (std::unique_ptr<Component>& component : entity->getComponents()) {
				if (ImGui::TreeNode(component->name().c_str())) {
					component->registerImGui();
					ImGui::TreePop();
				}
			}

			ImGui::TreePop();
		}
	}
	ImGui::Selectable("");
	ImGui::Text("Systems:");
	for (std::unique_ptr<System>& system : Engine::getInstance().getSystems()) {
		if (ImGui::TreeNode(system->name().c_str())) {
			ImGui::Text("Actions:");
			system->registerImGui();
			ImGui::TreePop();
		}
	}

	ImGui::End();
}

void ImGuiSystem::registerImGui() {}
