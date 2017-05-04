// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "textcomponent.hpp"

#include <cstring>

#include <iostream>

TextComponent::~TextComponent() {}

void TextComponent::registerImGui() {
	if (!textRenderer)
		return;
	static char* buffer = nullptr;
	static unsigned int length = (unsigned int)-1;
	if (!buffer || length != textRenderer->getMaxTextLength()) {
		length = textRenderer->getMaxTextLength();
		if (buffer)
			delete[] buffer;
		buffer = new char[length];
		memset(buffer, 0, length);
		strcpy(buffer, textRenderer->getText().c_str());
	}

	if (ImGui::InputText("Text", buffer, length, (textRenderer->isStatic() ? ImGuiInputTextFlags_ReadOnly : 0)))
		textRenderer->setText(std::string(buffer));

	if (ImGui::TreeNode("Transform")) {
		transform.registerImGui();
		ImGui::TreePop();
	}
}
