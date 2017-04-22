#pragma once

#include "component.hpp"

#include <glm/glm.hpp>

class Entity;
class State;

struct ButtonComponent : public Component<ButtonComponent> {
	typedef void (*ButtonCallback)(Entity& entity, State& state);

	ButtonComponent();

	glm::vec2 position;
	glm::vec2 size;
	ButtonCallback callback;

	virtual void registerImGui();

	virtual std::string name() { return "ButtonComponent"; }
};
