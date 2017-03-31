#pragma once

#include "component.hpp"

class RenderComponent : public Component<RenderComponent> {
public:
	virtual void render();
private:
};