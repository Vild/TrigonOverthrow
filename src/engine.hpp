#pragma once

#include <SDL2/SDL.h>
#include "lib/glad.h"
#include <glm/glm.hpp>
#include <exception>
#include <memory>
#include <cmath>

#include "io/texturemanager.hpp"
#include "io/meshloader.hpp"
#include "io/hidinput.hpp"
#include "io/textfactory.hpp"

#include "world/world.hpp"
#include "world/system/particlesystem.hpp"

#include "world/entity/cameraentity.hpp"

class Engine {
public:
	static Engine& getInstance() {
		static Engine instance;
		return instance;
	}

	Engine(Engine const&) = delete;
	void operator=(Engine const&) = delete;

	int run(bool vsync);

	inline unsigned int& getWidth() { return _width; }
	inline unsigned int& getHeight() { return _height; }
	inline SDL_Window* getWindow() { return _window; }

	inline std::shared_ptr<TextureManager> getTextureManager() { return _textureManager; }
	inline std::shared_ptr<MeshLoader> getMeshLoader() { return _meshLoader; }
	inline std::shared_ptr<HIDInput> getHIDInput() { return _hidInput; }
	inline std::shared_ptr<TextFactory> getTextFactory() { return _textFactory; }

	inline std::shared_ptr<World> getWorld() { return _world; }
	inline std::shared_ptr<CameraEntity> getCamera() { return _camera; }

private:
	unsigned int _width = 1280;
	unsigned int _height = 720;
	bool _vsync = true;

	bool _quit;
	SDL_Window* _window;
	SDL_GLContext _context;

	std::shared_ptr<TextureManager> _textureManager;
	std::shared_ptr<MeshLoader> _meshLoader;
	std::shared_ptr<HIDInput> _hidInput;
	std::shared_ptr<TextFactory> _textFactory;

	std::shared_ptr<World> _world;
	std::shared_ptr<CameraEntity> _camera;

	Engine() {}
	virtual ~Engine();

	void _init(bool vsync);
	void _initSDL();
	void _initGL();
	void _initImGui();
};
