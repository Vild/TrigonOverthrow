#pragma once

#include "component.hpp"

class Entity;

struct ButtonComponent : public Component<ButtonComponent> {
	typedef void (*ButtonCallback)(Entity* entity);

	ButtonComponent();

	glm::vec2 position;
	glm::vec2 size;
	ButtonCallback callback;

	virtual void registerImGui();

	virtual std::string name() { return "ButtonComponent"; }
};