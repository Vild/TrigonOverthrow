#pragma once

#include "component.hpp"

#include "../../io/textfactory.hpp"

struct TextComponent : public Component<TextComponent> {
	std::shared_ptr<TextRenderer> textRenderer;

	virtual void registerImGui();

	virtual std::string name() { return "TextComponent"; }
};
