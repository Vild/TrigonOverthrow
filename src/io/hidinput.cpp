// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "hidinput.hpp"

#include "../lib/imgui.h"
#include <SDL2/SDL.h>

#include "../engine.hpp"

void HIDInput::update() {
	/*auto& engine = Engine::getInstance();
	if (engine.getUpdateCamera()) {
		int x, y;
		SDL_GetMouseState(&x, &y);
		SDL_WarpMouseInWindow(engine.getWindow(), engine.getWidth() / 2, engine.getHeight() / 2);

		_xyDiff = glm::ivec2(engine.getWidth() / 2 - x, engine.getHeight() / 2 - y);
	} else*/
	_xyDiff = glm::ivec2(0, 0);

	_direction = glm::vec3(0);
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureKeyboard)
		return;

	const uint8_t* state = SDL_GetKeyboardState(NULL);

	if (state[SDL_SCANCODE_W])
		_direction.z++;
	if (state[SDL_SCANCODE_S])
		_direction.z--;

	if (state[SDL_SCANCODE_A])
		_direction.x--;
	if (state[SDL_SCANCODE_D])
		_direction.x++;

	if (state[SDL_SCANCODE_SPACE])
		_direction.y++;
	if (state[SDL_SCANCODE_LCTRL])
		_direction.y--;
}
