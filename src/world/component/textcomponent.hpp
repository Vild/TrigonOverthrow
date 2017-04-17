#pragma once

#include "component.hpp"

#include "transformcomponent.hpp"

#include "../../io/textfactory.hpp"
#include <glm/glm.hpp>

struct TextComponent : public Component<TextComponent> {
	std::shared_ptr<TextRenderer> textRenderer;

	// XXX: Hack, fix?
	TransformComponent transform;

	virtual void registerImGui();

	virtual std::string name() { return "TextComponent"; }
};
