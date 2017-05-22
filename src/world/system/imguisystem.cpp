// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "imguisystem.hpp"

#include "../../lib/imgui.h"
#include "../../engine.hpp"

ImGuiSystem::~ImGuiSystem() {}

void ImGuiSystem::update(World& world, float delta) {
	Engine& engine = Engine::getInstance();
	State& state = engine.getState();

	ImGui::SetNextWindowPos(ImVec2(8, 48), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(384, 512), ImGuiSetCond_Once);
	ImGui::SetNextWindowCollapsed(true, ImGuiSetCond_Once);
	ImGui::Begin("Settings Window");

	ImGui::Checkbox("Pause world", &engine.getPause());

	ImGui::Text("State:");
	state.registerImGui();

	ImGui::Text("Entities:");
	char name[255] = {0};
	for (std::unique_ptr<Entity>& entity : world.getEntities()) {
		if (entity->getHide())
			continue;
		ImGui::PushID(entity.get());
		snprintf(name, sizeof(name), "%s", entity->getName().c_str());
		if (ImGui::TreeNode(name)) {
			ImGui::Text("Actions:");
			if (entity->registerImGui)
				entity->registerImGui(*entity.get(), state);

			ImGui::Text("Component:");
			for (auto& component : entity->getComponents()) {
				if (ImGui::TreeNode(component.second->name().c_str())) {
					component.second->registerImGui();
					ImGui::TreePop();
				}
			}

			ImGui::TreePop();
		}
		ImGui::PopID();
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
