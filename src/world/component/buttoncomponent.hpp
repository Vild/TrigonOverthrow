#pragma once

#include "component.hpp"

#include <glm/glm.hpp>

#include "../../io/hidinput.hpp"

class Entity;
class State;

struct ButtonComponent : public Component {
	typedef void (*ButtonCallback)(Entity* entity, State& state, MouseState mouse);

	ButtonComponent();
	ButtonComponent(const ComponentValues& value);
	virtual ~ButtonComponent();

	glm::vec3 position;
	glm::vec3 size;
	ButtonCallback callback;

	void registerImGui() final;

	inline std::string name() final { return "ButtonComponent"; }
};
