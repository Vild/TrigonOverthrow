#pragma once

#include "component.hpp"

#include "../../utils/collisionbox.hpp"

struct ButtonComponent : public Component<ButtonComponent> {
	ButtonComponent();

	CollisionBox2D box;

	virtual void registerImGui();

	virtual std::string name() { return "ButtonComponent"; }
};