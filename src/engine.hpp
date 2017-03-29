#pragma once

#include <SDL2/SDL.h>
#include "lib/glad.h"
#include <glm/glm.hpp>
#include <exception>
#include <memory>
#include <cmath>

#include "io/texturemanager.hpp"

class Engine {
public:
	static Engine& getInstance() {
		static Engine instance;
		return instance;
	}

	Engine(Engine const&) = delete;
	void operator=(Engine const&) = delete;

	int run();

	std::shared_ptr<TextureManager> getTextureManager();

private:
	uint32_t _width = 1280;
	uint32_t _height = 720;
	bool _vsync = true;

	float _speed = 5.0f;
	float _fov = 80.0f;

	float _yaw = 0; // +Z (at 0, 0, 0)
	float _pitch = 0.0f;

	glm::vec3 _position = glm::vec3(0, 0, -2);

	glm::mat4 _projection;
	glm::mat4 _view;

	bool _quit;
	SDL_Window* _window;
	SDL_GLContext _context;

	std::shared_ptr<TextureManager> _textureManager;

	Engine() {}
	virtual ~Engine();

	void _init();
	void _initSDL();
	void _initGL();
	void _initImGui();
	void _initShaders();

	void _resolutionChanged();
	void _updateMovement(float delta, bool updateCamera);
	void _updateLights();
};
