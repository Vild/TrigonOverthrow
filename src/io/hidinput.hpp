#pragma once

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

enum MouseState { none = 0, left = 1, middle = 2, right = 4 };

inline MouseState operator|(const MouseState& a, const MouseState& b) {
	return static_cast<MouseState>((int)a | (int)b);
}

inline MouseState& operator|=(MouseState& a, const MouseState& b) {
	a = a | b;
	return a;
}

class HIDInput {
public:
	HIDInput();

	void update();

	inline bool& lockMouseCenter();
	inline bool getKey(SDL_Scancode key) { return !_keyboardBlock && !!_kbState[key]; }
	inline const glm::ivec2& getXY() { return _xy; }
	inline const glm::ivec2& getXYDiff() { return _xyDiff; }
	inline const MouseState& getMouseState() { return _mouseState; }

private:
	const uint8_t* _kbState = NULL;
	int _keyCount;
	bool _lockMouseCenter = false;
	bool _keyboardBlock = false;

	glm::ivec2 _xy{0, 0};
	glm::ivec2 _xyDiff{0, 0};
	MouseState _mouseState;
};
