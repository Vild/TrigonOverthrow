#pragma once

#include "state.hpp"

class MainMenuState : public State {
public:
	MainMenuState();

	virtual void onEnter(State* prev);
	virtual void onLeave(State* next);

private:
	Entity* _camera;
	Entity* _title;
	Entity* _play;
	Entity* _quit;
};
