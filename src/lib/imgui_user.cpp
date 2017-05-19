#include "imgui_user.h"

bool ImGui::ButtonWithColor(const char* label, const ImVec2& size, const ImVec4& buttonColor, const ImVec4& buttonColorHover, const ImVec4& buttonColorActive) {
	PushStyleColor(ImGuiCol_Button, buttonColor);
	PushStyleColor(ImGuiCol_ButtonHovered, buttonColorHover);
	PushStyleColor(ImGuiCol_ButtonActive, buttonColorActive);
	bool result = Button(label, size);

	PopStyleColor(3);
	return result;
}