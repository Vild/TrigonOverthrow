#pragma once

#include "imgui.h"

namespace ImGui {
bool ButtonWithColor(const char* label,
										 const ImVec2& size = ImVec2(0, 0),
										 const ImVec4& buttonColor = ImVec4(0.5, 0.5, 0.5, 1),
										 const ImVec4& buttonColorHover = ImVec4(0.6, 0.6, 0.6, 1),
										 const ImVec4& buttonColorActive = ImVec4(0.25, 0.25, 0.25, 1));
}
