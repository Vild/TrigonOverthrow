#pragma once

#include "state.hpp"

class InGameState : public State {
public:
	InGameState();
	virtual ~InGameState();

	void onEnter(State* prev) final;
	void onLeave(State* next) final;
	inline Entity* getCamera() final { return _camera; }
	inline Entity* getPlayer() final { return _player; }

	void registerImGui() final;

private:
	Entity* _sun;
	Entity* _camera;
	Entity* _player;
	Entity* _floor;
	Entity* _enemy;

	void _addLookAt();

	static void _registerImGUI(Entity& self, State& state);
	static void _updateUI(State& state, bool show);
	static void _menuRenderUI(Entity& self, State& state);
};
