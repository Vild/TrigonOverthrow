#pragma once

#include "state.hpp"

#include "../io/hidinput.hpp"

class MainMenuState : public State {
public:
	MainMenuState();
	virtual ~MainMenuState();

	void onEnter(State* prev) final;
	void onLeave(State* next) final;
	inline Entity* getCamera() final { return _camera; }
	inline Entity* getPlayer() final { return _target; }

	void registerImGui() final;

private:
	Entity* _camera;
	Entity* _target;
	Entity* _title;

	ImFont* _buttonFont;
};
