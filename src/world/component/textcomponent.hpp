#pragma once

#include "component.hpp"

#include "transformcomponent.hpp"

#include "../../io/textfactory.hpp"
#include <glm/glm.hpp>

struct TextComponent : public Component {
	std::shared_ptr<TextRenderer> textRenderer;

	TransformComponent transform;

	TextComponent() = default;
	TextComponent(const ComponentValues& value);
	virtual ~TextComponent();

	void registerImGui() final;

	inline std::string name() final { return "TextComponent"; }
};
