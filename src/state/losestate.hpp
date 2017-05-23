#pragma once

#include "state.hpp"

class LoseState : public State {
public:
	LoseState();
	virtual ~LoseState();

	void onEnter(State* prev) final;
	void onLeave(State* next) final;
	inline Entity* getCamera() final { return _camera; }
	inline Entity* getPlayer() final { return _target; }

	void registerImGui() final;

private:
	Entity* _camera;
	Entity* _target;
	Entity* _title;
};
