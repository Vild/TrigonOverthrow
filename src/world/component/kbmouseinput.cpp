#include "kbmouseinput.hpp"

#include "../../lib/imgui.h"
#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include "../../engine.hpp"

void KBMouseInputComponent::update() {
	auto& engine = Engine::getInstance();
	if (engine.getUpdateCamera()) {
		int x, y;
		SDL_GetMouseState(&x, &y);
		SDL_WarpMouseInWindow(engine.getWindow(), engine.getWidth() / 2, engine.getHeight() / 2);

		xy = glm::ivec2(x, y);

		xyDiff = glm::ivec2(engine.getWidth() / 2, engine.getHeight() / 2) - xy;
	} else
		xyDiff = glm::ivec2(0, 0);

	direction = glm::vec3(0);
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureKeyboard)
		return;

	const uint8_t* state = SDL_GetKeyboardState(NULL);

	if (state[SDL_SCANCODE_W])
		direction.z++;
	if (state[SDL_SCANCODE_S])
		direction.z--;

	if (state[SDL_SCANCODE_A])
		direction.x--;
	if (state[SDL_SCANCODE_D])
		direction.x++;

	if (state[SDL_SCANCODE_SPACE])
		direction.y++;
	if (state[SDL_SCANCODE_LCTRL])
		direction.y--;
}