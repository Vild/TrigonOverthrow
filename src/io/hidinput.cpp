// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "hidinput.hpp"

#include "../lib/imgui.h"
#include <SDL2/SDL.h>

#include "../engine.hpp"

HIDInput::HIDInput() {
	_kbState = SDL_GetKeyboardState(&_keyCount);
}

HIDInput::~HIDInput() {}

void HIDInput::update() {
	auto& engine = Engine::getInstance();

	int x, y;
	int buttonState = SDL_GetMouseState(&x, &y);
	if (_lockMouseCenter) {
		SDL_WarpMouseInWindow(engine.getWindow(), engine.getWidth() / 2, engine.getHeight() / 2);
		_xyDiff = glm::ivec2(engine.getWidth() / 2 - x, engine.getHeight() / 2 - y);
	} else
		_xyDiff = glm::ivec2(_xy.x - x, _xy.y - y);

	_xy = glm::ivec2(x, y);

	ImGuiIO& io = ImGui::GetIO();

	_mouseState = MouseState::none;
	if (!io.WantCaptureMouse) {
		if (buttonState & SDL_BUTTON(SDL_BUTTON_LEFT))
			_mouseState |= MouseState::left;
		if (buttonState & SDL_BUTTON(SDL_BUTTON_MIDDLE))
			_mouseState |= MouseState::middle;
		if (buttonState & SDL_BUTTON(SDL_BUTTON_RIGHT))
			_mouseState |= MouseState::right;
	}

	_keyboardBlock = io.WantCaptureKeyboard;
}

bool& HIDInput::lockMouseCenter() {
	auto& engine = Engine::getInstance();
	SDL_WarpMouseInWindow(engine.getWindow(), engine.getWidth() / 2, engine.getHeight() / 2);
	return _lockMouseCenter;
}
