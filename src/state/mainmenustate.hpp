#pragma once

#include "state.hpp"

class MainMenuState : public State {
public:
	MainMenuState();

	virtual void onEnter(State* prev);
	virtual void onLeave(State* next);
	inline virtual Entity* getCamera() { return _camera; }

private:
	Entity* _camera;
	Entity* _title;
	Entity* _play;
	Entity* _quit;

	static void _onButtonCallback(Entity& entity, State& state);
};
