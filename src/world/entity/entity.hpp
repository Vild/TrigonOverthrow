#pragma once

#include <string>

// Kinda like GameObject - https://docs.unity3d.com/ScriptReference/GameObject.html

class Entity {
public:
	virtual void update(float delta);
	virtual const std::string & name();
};