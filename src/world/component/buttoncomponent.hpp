#pragma once

#include "component.hpp"

#include <glm/glm.hpp>

#include "../../io/hidinput.hpp"

class Entity;
class State;

struct ButtonComponent : public Component {
	typedef void (*ButtonCallback)(Entity* entity, State& state, MouseState mouse);

	ButtonComponent();
	virtual ~ButtonComponent();

	glm::vec3 position;
	glm::vec3 size;
	ButtonCallback callback;

	virtual void registerImGui();

	virtual std::string name() { return "ButtonComponent"; }
};
