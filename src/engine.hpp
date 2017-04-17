#pragma once

#include <SDL2/SDL.h>
#include "lib/glad.h"
#include <glm/glm.hpp>
#include <exception>
#include <memory>
#include <cmath>
#include <typeinfo>
#include <typeindex>
#include <unordered_map>

#include "io/texturemanager.hpp"
#include "io/meshloader.hpp"
#include "io/hidinput.hpp"
#include "io/textfactory.hpp"

#include "world/world.hpp"
#include "world/system/particlesystem.hpp"

#include "world/entity/cameraentity.hpp"

#include "state/state.hpp"

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

	inline std::shared_ptr<Entity>& getCamera() { return _camera; }
	inline std::vector<std::shared_ptr<System>>& getSystems() { return _systems; }

	template <typename T>
	inline std::unique_ptr<T>& getState() {
		return _states[std::type_index(typeid(T))];
	}

	inline const std::type_index getCurrentState() { return *_currentState; }

	template <typename T>
	inline void setCurrentState() {
		*_currentState = std::type_index(typeid(T));
	}

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
	std::vector<std::shared_ptr<System>> _systems;
	std::shared_ptr<Entity> _camera;

	std::unique_ptr<std::type_index> _currentState;
	std::unordered_map<std::type_index, std::unique_ptr<State>> _states;

	Engine() {}
	virtual ~Engine();

	void _init(bool vsync);
	void _initSDL();
	void _initGL();
	void _initImGui();

	void _setupSystems();

	void _system_tick(float delta);
	void _system_resize(unsigned int width, unsigned int height);
};
