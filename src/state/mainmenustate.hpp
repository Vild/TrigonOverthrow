#pragma once

#include "state.hpp"

#include "../io/hidinput.hpp"

class MainMenuState : public State {
public:
	MainMenuState();
	virtual ~MainMenuState();

	virtual void onEnter(State* prev);
	virtual void onLeave(State* next);
	inline virtual Entity* getCamera() { return _camera; }

private:
	Entity* _camera;
	Entity* _target;
	Entity* _title;
	Entity* _play;
	Entity* _quit;

	void _toggleBox(Entity& entity);

	static void _registerImGUI(Entity& self, State& state);
	static void _onButtonCallback(Entity* entity, State& state, MouseState mouse);
};
