#pragma once

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

class HIDInput {
public:
	HIDInput();

	void update();

	inline bool& lockMouseCenter();
	inline bool getKey(SDL_Scancode key) { return !_keyboardBlock && !!_kbState[key]; }
	inline const glm::ivec2& getXY() { return _xy; }
	inline const glm::ivec2& getXYDiff() { return _xyDiff; }

private:
	const uint8_t* _kbState = NULL;
	int _keyCount;
	bool _lockMouseCenter = false;
	bool _keyboardBlock = false;

	glm::ivec2 _xy{0, 0};
	glm::ivec2 _xyDiff{0, 0};
};
